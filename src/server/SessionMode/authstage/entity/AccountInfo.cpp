#include "AccountInfo.hpp"
#include "Logger.hpp"

void AccountInfo::setUserName(const std::string &username) {
    username_ = username;
    srp()->set_only_username(username);
}

void AccountInfo::handle_auth_state() {
    setIsAuthenticated(true);
    Logger::get()->info("Account {} was successfully authorized", srp()->getUserName());
}

void AccountInfo::handle_close_state() {
    Logger::get()->info("Account {} was successfully closed connection", srp()->getUserName());
}