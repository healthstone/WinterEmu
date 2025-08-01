#include "src/authserver/AuthSession/AuthSession.hpp"

/** AUTH_LOGON_CHALLENGE and AUTH_RECONNECT_CHALLENGE **/
namespace HandlersReconnectProofStage
{
    void HandleReconnectProof(const std::shared_ptr<AuthSession>& session, std::shared_ptr<std::vector<uint8_t>>& payload);

    bool VerifyVersion(const std::shared_ptr<AuthSession>& session, uint8_t const* a, int32_t aLength, Crypto::SHA1::Digest const& versionProof, bool isReconnect);
}
