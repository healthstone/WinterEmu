#include "HandlersRealmStage.hpp"
#include "src/authserver/Entity/AuthCodes/AuthCodes.hpp"
#include "src/authserver/Entity/ClientBuildInfo/ClientBuildInfo.hpp"
#include "packet/RawPacket.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "utils/PacketUtils.hpp"

using namespace HandlersRealmStage;

boost::asio::awaitable<void> HandlersRealmStage::HandleRealmList(
        std::shared_ptr<AuthSession> session,
        std::shared_ptr<std::vector<uint8_t>> payload) {
    try {
        std::map<uint32_t, uint8_t> characterCounts;

        PreparedStatement stmt1("SELECT_REALM_CHARACTERS");
        stmt1.set_param(0, session->_accountGUID);
        //Packet::log_raw_payload("REQUEST  REALM_LIST", *payload);

        auto rows = co_await session->server()->db()->execute_async_many<RealmCharactersRow>(stmt1);
        if (!rows.empty()) {
            for (const auto &row: rows) {
                characterCounts[row.realmid] = row.numchars;
            }
        } else {
            co_await fillInitialRealmCharacters(session);
        }

        RawPacket packet;
        size_t realmListSize = 0;

        const auto &realmsMap = session->server()->realm_list()->getRealmsMap();
        for (const auto &[id, realm]: realmsMap) {
            if (!realm) continue;

            // Работай через realm->
            bool okBuild =
                    ((session->_expversion & POST_BC_EXP_FLAG) && realm->Build == session->_logonChallenge.build) ||
                    ((session->_expversion & PRE_BC_EXP_FLAG) && !AuthHelper::IsPreBCAcceptedClientBuild(realm->Build));

            uint32_t flag = static_cast<uint32_t>(realm->Flags);

            const ClientBuild::Info *buildInfo = ClientBuild::GetBuildInfo(realm->Build);
            if (!okBuild) {
                if (!buildInfo) continue;
                flag |= static_cast<uint32_t>(RealmFlags::REALM_FLAG_OFFLINE) | static_cast<uint32_t>(RealmFlags::REALM_FLAG_SPECIFYBUILD);
            }

            if (!buildInfo) {
                flag &= ~static_cast<uint32_t>(RealmFlags::REALM_FLAG_SPECIFYBUILD);
            }

            std::string name = realm->Name;
            if ((session->_expversion & PRE_BC_EXP_FLAG) && (flag & static_cast<uint32_t>(RealmFlags::REALM_FLAG_SPECIFYBUILD))) {
                std::ostringstream ss;
                ss << name << " (" << buildInfo->MajorVersion
                   << '.' << buildInfo->MinorVersion
                   << '.' << buildInfo->BugfixVersion << ')';
                name = ss.str();
            }

            uint8_t lock = (realm->AllowedSecurityLevel > session->_securityLevel) ? 1 : 0;

            packet.write_uint8(static_cast<uint8_t>(realm->Type));
            if (session->_expversion & POST_BC_EXP_FLAG)
                packet.write_uint8(lock);

            packet.write_uint8(flag);
            packet.write_string_nt_be(name);

            boost::asio::ip::address clientAddr = session->GetRemoteIpAddress();
            boost::asio::ip::tcp::endpoint endpoint = realm->GetAddressForClient(clientAddr);
            packet.write_string_nt_be(fmt::format("{}:{}", endpoint.address().to_string(), endpoint.port()));

            packet.write_float_le(realm->PopulationLevel);
            packet.write_uint8(characterCounts[realm->Id]);
            packet.write_uint8(realm->Timezone);

            if (session->_expversion & POST_BC_EXP_FLAG)
                packet.write_uint8(realm->Id);
            else
                packet.write_uint8(0x0);

            if ((session->_expversion & POST_BC_EXP_FLAG) && (flag & static_cast<uint32_t>(RealmFlags::REALM_FLAG_SPECIFYBUILD))) {
                packet.write_uint8(buildInfo->MajorVersion);
                packet.write_uint8(buildInfo->MinorVersion);
                packet.write_uint8(buildInfo->BugfixVersion);
                packet.write_uint16_le(buildInfo->Build);
            }

            ++realmListSize;
        }

        if (session->_expversion & POST_BC_EXP_FLAG) {
            packet.write_uint8(0x10);
            packet.write_uint8(0x00);
        } else {
            packet.write_uint8(0x00);
            packet.write_uint8(0x02);
        }

        RawPacket realmListSizePacket;
        realmListSizePacket.write_uint32_le(0);
        if (session->_expversion & POST_BC_EXP_FLAG)
            realmListSizePacket.write_uint16_le(static_cast<uint16_t>(realmListSize));
        else
            realmListSizePacket.write_uint32_le(static_cast<uint32_t>(realmListSize));

        RawPacket hdr;
        hdr.write_uint8(static_cast<uint8_t>(AuthCmd::REALM_LIST));
        hdr.write_uint16_le(static_cast<uint16_t>(packet.size() + realmListSizePacket.size()));
        hdr.write_bytes(realmListSizePacket.serialize());
        hdr.write_bytes(packet.serialize());

        //session->set_session_mode(SessionMode::STATUS_AUTHED);
        //Packet::log_raw_payload("RESPONSE REALM_LIST", hdr.serialize());
        PacketUtils::send_packet_as<RawPacket>(std::move(session), hdr);
        co_return;

    } catch (const std::exception &ex) {
        Logger::get()->error("[HandleRealmList] exception: {}", ex.what());
        co_return;
    }
}

boost::asio::awaitable<void>
HandlersRealmStage::fillInitialRealmCharacters(std::shared_ptr<AuthSession> session) {
    try {
        const auto &realmsMap = session->server()->realm_list()->getRealmsMap();
        for (const auto &[id, realm]: realmsMap) {
            PreparedStatement stmt("INSERT_REALM_CHARACTERS");
            stmt.set_param(0, id);
            stmt.set_param(1, session->_accountGUID);
            stmt.set_param(2, 0);
            co_await session->server()->db()->execute_async_one<NothingRow>(stmt);
        }
    } catch (const std::exception &ex) {
        Logger::get()->error("[HandlersRealmStage][fillInitialRealmCharacters] exception: {}", ex.what());
    }
}