/** AUTH_LOGON_CHALLENGE and AUTH_RECONNECT_CHALLENGE **/
#include "src/authserver/ClientSession/ClientSession.hpp"

namespace HandlersReconnectProofStage
{
    void HandleReconnectProof(const std::shared_ptr<ClientSession>& session, const std::vector<uint8_t>& payload);

    bool VerifyVersion(const std::shared_ptr<ClientSession>& session, uint8_t const* a, int32_t aLength, Crypto::SHA1::Digest const& versionProof, bool isReconnect);
}
