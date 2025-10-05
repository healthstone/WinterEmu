// CrashHandler.cpp
#include "CrashHandler.hpp"
#include <boost/stacktrace.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>

namespace {

    void signal_handler(int sig) {
        std::cerr << "\n🚨🚨🚨 CRASH DETECTED 🚨🚨🚨" << std::endl;
        std::cerr << "Signal: " << sig << std::endl;
        std::cerr << "Detailed stacktrace:" << std::endl;

        try {
            auto trace = boost::stacktrace::stacktrace();

            // Детальный вывод с номерами строк (если доступно)
            std::cerr << "Stacktrace (with source locations if available):\n";
            std::cerr << trace << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Failed to generate stacktrace: " << e.what() << std::endl;
        }

        std::_Exit(1);
    }

} // namespace

namespace CrashHandler {
    void setup() {
        std::signal(SIGSEGV, signal_handler);
        std::signal(SIGABRT, signal_handler);
        std::signal(SIGFPE,  signal_handler);
        std::signal(SIGILL,  signal_handler);
        std::signal(SIGBUS,  signal_handler);
        std::signal(SIGTERM, signal_handler);
    }
}