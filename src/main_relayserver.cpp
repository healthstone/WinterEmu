#include "relayserver/RelayServer.hpp"
#include "Database.hpp"
#include "Logger.hpp"
#include "CrashHandler/CrashHandler.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <csignal>
#include <memory>
#include <thread>
#include <vector>

int main() {
    CrashHandler::setup();
    Logger::init_thread_pool();
    auto log = Logger::get();

    try {
        unsigned int max_threads = std::thread::hardware_concurrency();
        unsigned int async_threads = std::getenv("RELAY_DB_ASYNC_THREADS") ?
                                     static_cast<unsigned int>(std::stoi(std::getenv("RELAY_DB_ASYNC_THREADS"))) : 2;

        unsigned int network_threads = max_threads > async_threads ? max_threads - async_threads : 1;
        if (network_threads > 2) network_threads -= 1;
        else if (network_threads == 0) network_threads = 1;

        if (std::getenv("RELAY_NETWORK_THREADS"))
            network_threads = static_cast<unsigned int>(std::stoi(std::getenv("RELAY_NETWORK_THREADS")));

        int port = std::getenv("RELAY_PORT") ? std::stoi(std::getenv("RELAY_PORT")) : 8085;
        uint32_t realmID = std::getenv("REALM_ID") ? std::stoi(std::getenv("REALM_ID")) : 1;

        boost::asio::io_context io_context;
        auto work_guard = boost::asio::make_work_guard(io_context);

        // Настройка БД
        auto db = std::make_shared<Database>(
                fmt::format("host={} port={} user={} password={} dbname={}",
                            std::getenv("DB_URL") ?: "127.0.0.1",
                            std::getenv("DB_PORT") ?: "5432",
                            std::getenv("DB_USER") ?: "postgres",
                            std::getenv("DB_PASSWORD") ?: "postgres",
                            std::getenv("DB_NAME") ?: "postgres"),
                network_threads,
                async_threads
        );

        // RelayServer
        auto server = std::make_shared<RelayServer>(io_context, db, port);
        server->init(network_threads, realmID);
        server->start_accept();
        log->info("[RelayServer] Running on port {} with {} network threads", port, network_threads);

        // Используем weak_ptr, чтобы не держать лишнюю shared ссылку
        std::weak_ptr<RelayServer> weak_server(server);

        // Сигналы
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([weak_server, &work_guard, &log](const boost::system::error_code &, int signal_number) {
            log->info("[RelayServer] Signal {} received, shutting down...", signal_number);

            if (auto srv = weak_server.lock()) {
                srv->stop();       // корректная остановка сервера
            }

            work_guard.reset();   // снимаем guard — io_context.run() завершится
        });

        // Потоки io_context
        std::vector<std::thread> threads;
        threads.reserve(network_threads);
        for (unsigned int i = 0; i < network_threads; ++i)
            threads.emplace_back([&io_context]() { io_context.run(); });

        // Главный поток тоже запускает io_context
        io_context.run();

        // Ждём потоков
        for (auto &t: threads) t.join();

        // Явно обнуляем server и db
        server.reset();
        db->shutdown();
        db.reset();

        log->info("[RelayServer] Gracefully shut down.");
    } catch (const std::exception &e) {
        log->error("[RelayServer] Exception: {}", e.what());
    }

    spdlog::shutdown();
    return 0;
}
