#include <cstdint>
#include <memory>
#include "srp6/SRP6.hpp"

class AccountInfo {
public:
    AccountInfo() = default;

    ~AccountInfo() = default;

    SRP6 *srp() { return srp_.get(); }

    void setIsAuthenticated(bool value) { isAuth = value; }

    bool isAuthenticated() { return isAuth; }

    void setUserName(const std::string &username);
    std::string getUserName() {return username_;}

    /** метод, вызываемый, когда acc становится авторизованным **/
    void handle_auth_state();

    /** метод, вызываемый, когда acc завершает соединение **/
    void handle_close_state();

private:
    std::shared_ptr<SRP6> srp_ = std::make_shared<SRP6>();
    bool isAuth = false;
    std::string username_;
};