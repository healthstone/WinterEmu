#include "RelayServer.hpp"
#include "GameSession/GameSession.hpp"
#include "Logger.hpp"

using boost::asio::ip::tcp;

RelayServer::RelayServer(boost::asio::io_context &io_context,
                         std::shared_ptr<Database> db,
                         int port)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          db_(std::move(db)) {
}

void RelayServer::start_accept() {
    if (!acceptor_.is_open()) return;

    acceptor_.async_accept(
            [self = shared_from_this()](boost::system::error_code ec, tcp::socket socket) {
                auto log = Logger::get();
                if (ec) {
                    if (ec != boost::asio::error::operation_aborted &&
                        ec != boost::asio::error::eof) {
                        log->error("[RelayServer] Accept failed: {}", ec.message());
                    }
                    return;
                }

                auto session = std::make_shared<GameSession>(std::move(socket), self);

                {
                    std::lock_guard<std::mutex> lock(self->sessions_mutex_);
                    self->sessions_.insert(session);
                    log->info("[RelayServer] New client connected.");
                    self->log_session_count();
                }

                session->start();
                self->start_accept();
            }
    );
}

void RelayServer::stop() {
    auto log = Logger::get();

    if (node_manager_) {
        node_manager_->stop_all();
    }

    boost::system::error_code ec;
    acceptor_.cancel(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[RelayServer] Failed to cancel acceptor: {}", ec.message());
    }

    acceptor_.close(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[RelayServer] Failed to close acceptor: {}", ec.message());
    }

    // Для избежания dead lock'a, нужно делать копию списка, закрыть открытые сокеты (где тоже мьютекс)
    {
        std::unordered_set<std::shared_ptr<GameSession>> sessions_copy;
        {
            std::lock_guard<std::mutex> lock(sessions_mutex_);
            sessions_copy = sessions_;
        }

        for (auto &s: sessions_copy) {
            if (s->isOpened()) s->close();
        }
    }

    // Теперь очищаем список
    {
        std::lock_guard<std::mutex> lock(sessions_mutex_);
        sessions_.clear();
    }

    io_context_.stop();

    // ✅ Корректно закрываем все DB connections:
    if (db_) db_->shutdown();

    log_session_count();
}

void RelayServer::remove_session(std::shared_ptr<GameSession> session) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    sessions_.erase(session);
    log_session_count();
}

void RelayServer::log_session_count() {
    Logger::get()->info("[RelayServer] Active sessions: {}", sessions_.size());
}

void RelayServer::init(unsigned int network_threads) {
    // Загружаем различные данные
    node_manager_ = std::make_unique<NodeManager>(io_context_);
    node_manager_->add_connectors(1, "127.0.0.1", 8086, network_threads);
    node_manager_->start_all();
}