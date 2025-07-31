#include "AccountInfo.hpp"
#include "Logger.hpp"

void AccountInfo::handle_auth_state() {
    Logger::get()->info("Account {} was successfully authorized", Login);
}

void AccountInfo::handle_close_state() {
    Logger::get()->info("Account {} was successfully closed connection", Login);
}
