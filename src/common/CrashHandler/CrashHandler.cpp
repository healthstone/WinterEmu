// CrashHandler.cpp
#include "CrashHandler.hpp"
#include "Logger.hpp"

#include <boost/stacktrace.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>

namespace {

    void signal_handler(int sig) {
        auto log = Logger::get();
        log->error("===== CRASH (signal {}) =====", sig);
        log->error("Stacktrace:\n{}", boost::stacktrace::to_string(boost::stacktrace::stacktrace()));
        spdlog::shutdown();
        std::_Exit(1); // мгновенный выход, без разрушения стека
    }

} // namespace

namespace CrashHandler {
    void setup() {
        std::signal(SIGSEGV, signal_handler);
        std::signal(SIGABRT, signal_handler);
        std::signal(SIGFPE,  signal_handler);
        std::signal(SIGILL,  signal_handler);
        std::signal(SIGTERM, signal_handler);
    }
}
