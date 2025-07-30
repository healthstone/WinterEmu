#include "AccountInfo.hpp"
#include "Logger.hpp"

void AccountInfo::setUserName(const std::string &username) {
    username_ = username;
}

void AccountInfo::handle_auth_state() {
    Logger::get()->info("Account {} was successfully authorized", username_);
}

void AccountInfo::handle_close_state() {
    Logger::get()->info("Account {} was successfully closed connection", username_);
}
