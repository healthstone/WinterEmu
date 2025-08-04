#include "relayserver/RelayServer.hpp"
#include "Database.hpp"
#include "Logger.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <csignal>

int main() {
    Logger::init_thread_pool();  // Инициализировать thread pool до первого лога!
    auto log = Logger::get();

    try {
        // all possible threads on machine
        unsigned int max_threads = std::thread::hardware_concurrency();

        // read env DB_ASYNC_THREADS
        unsigned int async_threads = std::getenv("RELAY_DB_ASYNC_THREADS") ? static_cast<unsigned int>(std::stoi(std::getenv("RELAY_DB_ASYNC_THREADS"))) : 2; // default 2

        // set default network_threads (max - async threads)
        unsigned int network_threads = max_threads - async_threads;
        if (network_threads > 2) network_threads -= 1;              // default: max - async ( for example if 8 max: 2 async and 5 network  and 1 for system and logs )
        else if (network_threads == 0) network_threads = 1;

        // read env NETWORK_THREADS
        if (std::getenv("RELAY_NETWORK_THREADS"))
            network_threads = static_cast<unsigned int>(std::stoi(std::getenv("RELAY_NETWORK_THREADS")));

        // read env AUTH_PORT
        int port = std::getenv("RELAY_PORT") ? static_cast<int>(std::stoi(std::getenv("RELAY_PORT"))) : 8085;

        // 🟢 Используем только io_context
        boost::asio::io_context io_context;

        // 🟢 Настройка БД
        auto db = std::make_shared<Database>(
                fmt::format("host={} port={} user={} password={} dbname={}",
                            std::getenv("DB_URL") ?: "127.0.0.1",
                            std::getenv("DB_PORT") ?: "5432",
                            std::getenv("DB_USER") ?: "postgres",
                            std::getenv("DB_PASSWORD") ?: "postgres",
                            std::getenv("DB_NAME") ?: "postgres"),
                network_threads,   // Для каждого потока должна быть своя сессия к бд
                async_threads      // Для асинхронных запросов должен быть выделен минимум 1 поток
        );

        auto server = std::make_shared<RelayServer>(io_context, db, port);
        server->init(network_threads);
        server->start_accept();
        log->info("[RelayServer] Running on port {} with {} network threads", port, network_threads);

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](const boost::system::error_code &, int signal_number) {
            log->info("[RelayServer] Signal {} received, shutting down...", signal_number);
            server->stop();
            db->shutdown();
        });

        std::vector<std::thread> threads;
        for (unsigned int i = 0; i < network_threads; ++i) {
            threads.emplace_back([&io_context]() {
                io_context.run();
            });
        }

        for (auto &t : threads) t.join();

        log->info("[RelayServer] Gracefully shut down.");
    } catch (const std::exception &e) {
        log->error("[RelayServer] Exception: {}", e.what());
    }

    spdlog::shutdown();
    return 0;
}
