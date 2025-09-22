#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "Logger.hpp"

// RAII guard, который гарантирует init + shutdown
struct LoggerGuard {
    LoggerGuard() {
        Logger::init_thread_pool();
        Logger::get()->info("Logger initialized for tests");
    }

    ~LoggerGuard() {
        Logger::get()->info("Logger shutdown requested");
        Logger::shutdown();
    }
};

// Глобальный guard запускается до main и завершает после main
LoggerGuard logger_guard;