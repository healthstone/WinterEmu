#include "ReaderRealmStage.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/SessionMode/RealmStage/handlers/HandlersRealmStage.hpp"

using namespace ReaderRealmStage;

#define REALM_LIST_PACKET_SIZE 5

/** REALM_LIST **/
void ReaderRealmStage::process_read_buffer(std::shared_ptr<ClientSession> session)
{
    auto& buffer = session->read_buffer();

    constexpr size_t packet_size = REALM_LIST_PACKET_SIZE;
    if (buffer.get_active_size() < packet_size)
        return;

    const uint8_t* data = buffer.read_ptr();

    // Проверяем, что cmd соответствует REALM_LIST
    if (data[0] != static_cast<uint8_t>(AuthCmd::REALM_LIST))
    {
        Logger::get()->warn("ReaderAuthStage: Unexpected cmd 0x{:02X}, expected REALM_LIST", data[0]);
        return;
    }

    std::vector<uint8_t> payload(data, data + packet_size);
    buffer.read_completed(packet_size);

    boost::asio::co_spawn(
            session->socket().get_executor(),
            HandlersRealmStage::HandleRealmList(session, payload),
            boost::asio::detached
    );
}