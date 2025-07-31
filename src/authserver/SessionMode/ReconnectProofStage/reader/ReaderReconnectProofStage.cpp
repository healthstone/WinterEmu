#include "ReaderReconnectProofStage.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/SessionMode/ReconnectProofStage/handlers/HandlersReconnectProofStage.hpp"

using namespace ReaderReconnectProofStage;

void ReaderReconnectProofStage::process_read_buffer(std::shared_ptr<ClientSession> session) {
    auto& buffer = session->read_buffer();

    constexpr size_t packet_size = sizeof(uint8_t)    // cmd
                                   + 16                  // R1[16];
                                   + 20                  // R2
                                   + 20                  // R3
                                   + 1;                  // number_of_keys
    // или просто 75, если не хочется писать явно

    if (buffer.get_active_size() < packet_size)
        return;

    const uint8_t* data = buffer.read_ptr();

    // Проверяем, что cmd соответствует AUTH_LOGON_PROOF
    if (data[0] != static_cast<uint8_t>(AuthCmd::AUTH_RECONNECT_PROOF)) {
        Logger::get()->warn("ReaderLogonProofStage: Unexpected cmd 0x{:02X}, expected AUTH_RECONNECT_PROOF", data[0]);
        // Можно закрыть сессию или очистить буфер, но пока просто возвращаемся
        return;
    }

    std::vector<uint8_t> payload(data, data + packet_size);
    buffer.read_completed(packet_size);
    HandlersReconnectProofStage::HandleReconnectProof(session, payload);
}