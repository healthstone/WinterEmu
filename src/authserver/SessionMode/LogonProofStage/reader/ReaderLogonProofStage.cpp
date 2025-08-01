#include "ReaderLogonProofStage.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/SessionMode/LogonProofStage/handlers/HandlersLogonProofStage.hpp"

using namespace ReaderLogonProofStage;

void ReaderLogonProofStage::process_read_buffer(const std::shared_ptr<AuthSession>& session) {
    auto& buffer = session->read_buffer();

    constexpr size_t packet_size = sizeof(uint8_t)    // cmd
                                   + 32                  // A
                                   + 20                  // clientM
                                   + 20                  // crc_hash
                                   + 1                   // number_of_keys
                                   + 1;                  // securityFlags
    // или просто 75, если не хочется писать явно

    if (buffer.get_active_size() < packet_size)
        return;

    const uint8_t* data = buffer.read_ptr();

    // Проверяем, что cmd соответствует AUTH_LOGON_PROOF
    if (data[0] != static_cast<uint8_t>(AuthCmd::AUTH_LOGON_PROOF)) {
        Logger::get()->warn("ReaderLogonProofStage: Unexpected cmd 0x{:02X}, expected AUTH_LOGON_PROOF", data[0]);
        // Можно закрыть сессию или очистить буфер, но пока просто возвращаемся
        return;
    }

    auto payload = std::make_shared<std::vector<uint8_t>>(data, data + packet_size);
    buffer.read_completed(packet_size);

    boost::asio::co_spawn(
            session->socket().get_executor(),
            HandlersLogonProofStage::HandleLogonProof(session, payload),
            boost::asio::detached
    );
}