#include "AuthServer.hpp"
#include "AuthSession/AuthSession.hpp"
#include "Logger.hpp"
#include "src/authserver/Entity/ClientBuildInfo/ClientBuildInfo.hpp"

using boost::asio::ip::tcp;

AuthServer::AuthServer(boost::asio::io_context &io_context,
                       std::shared_ptr<Database> db,
                       int port)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          db_(std::move(db)) {
}

void AuthServer::start_accept() {
    if (!acceptor_.is_open()) return;

    acceptor_.async_accept(
            [self = shared_from_this()](boost::system::error_code ec, tcp::socket socket) {
                auto log = Logger::get();
                if (ec) {
                    if (ec != boost::asio::error::operation_aborted &&
                        ec != boost::asio::error::eof) {
                        log->error("[AuthServer] Accept failed: {}", ec.message());
                    }
                    return;
                }

                auto session = std::make_shared<AuthSession>(std::move(socket), self);

                {
                    std::lock_guard<std::mutex> lock(self->sessions_mutex_);
                    self->sessions_.insert(session);
                    log->info("[AuthServer] New client connected.");
                    self->log_session_count();
                }

                session->start();
                self->start_accept();
            }
    );
}

void AuthServer::stop() {
    auto log = Logger::get();

    // Выгружаем различные данные
    ClientBuild::Clear();

    boost::system::error_code ec;
    acceptor_.cancel(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[AuthServer] Failed to cancel acceptor: {}", ec.message());
    }

    acceptor_.close(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[AuthServer] Failed to close acceptor: {}", ec.message());
    }

    // Для избежания dead lock'a, нужно делать копию списка, закрыть открытые сокеты (где тоже мьютекс)
    {
        std::unordered_set<std::shared_ptr<AuthSession>> sessions_copy;
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

void AuthServer::remove_session(std::shared_ptr<AuthSession> session) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    sessions_.erase(session);
    log_session_count();
}

void AuthServer::log_session_count() {
    Logger::get()->info("[AuthServer] Active sessions: {}", sessions_.size());
}

bool AuthServer::disconnectSessionIfExists(const std::string &username) {
    for (auto &session: sessions_) {
        if (session->_login == username) {
            Logger::get()->warn("[AuthServer] Found duplicate session for '{}', disconnecting old session.", username);
            session->close();  // Предположим, что у AuthSession есть метод close()
            return true;
        }
    }

    return false;
}

void AuthServer::init() {
    // Загружаем различные данные
    ClientBuild::LoadBuildInfo(db_);

    realmList_ = std::make_shared<RealmList>(io_context_, shared_from_this(), std::chrono::seconds(20));
    realmList_->load_realms(true);

    account_cache_ = std::make_shared<AccountCache>(io_context_, std::chrono::minutes(5), std::chrono::minutes(1));

    // Запускаем всякие таймеры
    account_cache_->start();
    realmList_->start();
}