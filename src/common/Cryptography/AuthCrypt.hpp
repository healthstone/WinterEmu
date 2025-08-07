#include "arc4/ARC4.hpp"
#include "AuthDefines.hpp"
#include <array>

class AuthCrypt {
public:
    AuthCrypt();

    void Init(SessionKey const &K);

    void DecryptRecv(uint8_t *data, size_t len);

    void EncryptSend(uint8_t *data, size_t len);

    bool IsInitialized() const { return _initialized; }

private:
    Crypto::ARC4 _clientDecrypt;
    Crypto::ARC4 _serverEncrypt;
    bool _initialized;
};