#include "Server.hpp"
#include "ClientSession/ClientSession.hpp"
#include "Logger.hpp"

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_context &io_context,
               std::shared_ptr<Database> db,
               int port)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          db_(std::move(db)),
          account_cache_(std::make_shared<AccountCache>(io_context, std::chrono::minutes(5), std::chrono::minutes(1)))
{
    account_cache_->start(); // <-- Запускаем таймер только после make_shared
}

void Server::start_accept() {
    if (!acceptor_.is_open()) return;

    acceptor_.async_accept(
            [self = shared_from_this()](boost::system::error_code ec, tcp::socket socket) {
                auto log = Logger::get();
                if (ec) {
                    if (ec != boost::asio::error::operation_aborted &&
                        ec != boost::asio::error::eof) {
                        log->error("[Server] Accept failed: {}", ec.message());
                    }
                    return;
                }

                auto session = std::make_shared<ClientSession>(std::move(socket), self);

                {
                    std::lock_guard<std::mutex> lock(self->sessions_mutex_);
                    self->sessions_.insert(session);
                    log->info("[Server] New client connected.");
                    self->log_session_count();
                }

                session->start();
                self->start_accept();
            }
    );
}

void Server::stop() {
    auto log = Logger::get();

    if (account_cache_) {
        account_cache_->stop();
    }

    boost::system::error_code ec;
    acceptor_.cancel(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[Server] Failed to cancel acceptor: {}", ec.message());
    }

    acceptor_.close(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[Server] Failed to close acceptor: {}", ec.message());
    }

    // Для избежания dead lock'a, нужно делать копию списка, закрыть открытые сокеты (где тоже мьютекс)
    {
        std::unordered_set<std::shared_ptr<ClientSession>> sessions_copy;
        {
            std::lock_guard<std::mutex> lock(sessions_mutex_);
            sessions_copy = sessions_;
        }

        for (auto &s : sessions_copy) {
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

void Server::remove_session(std::shared_ptr<ClientSession> session) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    sessions_.erase(session);
    log_session_count();
}

void Server::log_session_count() {
    Logger::get()->info("[Server] Active sessions: {}", sessions_.size());
}

bool Server::disconnectSessionIfExists(const std::string &username) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);

    for (auto &session : sessions_) {
        auto accountInfo = session->getAccountInfo();
        if (accountInfo && accountInfo->getUserName() == username) {
            Logger::get()->warn("[Server] Found duplicate session for '{}', disconnecting old session.", username);
            session->close();  // Предположим, что у ClientSession есть метод close()
            return true;
        }
    }

    return false;
}