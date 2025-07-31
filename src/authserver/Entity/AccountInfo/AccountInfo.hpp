#include <cstdint>
#include <memory>
#include "srp6/SRP6.hpp"
#include "src/authserver/enums/AccountTypes.hpp"
#include <boost/uuid/uuid.hpp>

class AccountInfo {
public:
    AccountInfo() = default;

    ~AccountInfo() = default;

    /** метод, вызываемый, когда acc становится авторизованным **/
    void handle_auth_state();

    /** метод, вызываемый, когда acc завершает соединение **/
    void handle_close_state();

    boost::uuids::uuid AccountID;
    std::string Login;
    std::string Email;
    bool IsLockedToIP = false;
    std::string LockCountry;
    std::string LastIP;
    uint32_t FailedLogins = 0;
    bool IsBanned = false;
    bool IsPermanenetlyBanned = false;
    AccountTypes SecurityLevel = AccountTypes::SEC_PLAYER;

private:
};