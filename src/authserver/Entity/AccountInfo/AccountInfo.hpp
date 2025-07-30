#include <cstdint>
#include <memory>
#include "srp6/SRP6.hpp"

class AccountInfo {
public:
    AccountInfo() = default;

    ~AccountInfo() = default;

    void setUserName(const std::string &username);
    std::string getUserName() {return username_;}

    /** метод, вызываемый, когда acc становится авторизованным **/
    void handle_auth_state();

    /** метод, вызываемый, когда acc завершает соединение **/
    void handle_close_state();

private:
    std::string username_;
};