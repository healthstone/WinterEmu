#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <deque>
#include <atomic>

#include "src/authserver/AuthServer.hpp"
#include "packet/MessageBuffer.hpp"
#include "packet/Packet.hpp"
#include "src/game/enums/AccountTypes.hpp"
#include "Cryptography/srp6/SRP6.hpp"
#include "src/authserver/SessionMode/ChallengeStage/handlers/LogonChallenge.hpp"

class AuthServer; // forward declaration

enum class SessionMode {
    STATUS_CHALLENGE = 0,
    STATUS_LOGON_PROOF,
    STATUS_RECONNECT_PROOF,
    STATUS_WAITING_FOR_REALM_LIST,
    STATUS_AUTHED,
    STATUS_CLOSED
};

class AuthSession : public std::enable_shared_from_this<AuthSession> {
public:
    AuthSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<AuthServer> server);

    void start();

    void close();

    bool isOpened() const { return !closed_; }

    void send_packet(const std::shared_ptr<const Packet>& packet);

    boost::asio::ip::tcp::socket &socket() { return socket_; }

    std::shared_ptr<AuthServer> server() const { return server_; }

    // Режим: AUTH или WORK
    void set_session_mode(SessionMode mode) { session_mode_ = mode; }

    SessionMode get_session_mode() const { return session_mode_; }

    MessageBuffer &read_buffer() {
        return read_buffer_;
    }

    boost::asio::ip::address GetRemoteIpAddress() const
    {
        boost::asio::ip::tcp::endpoint remote_ep = socket_.remote_endpoint();
        return remote_ep.address();
    }

    uint16_t GetRemotePort() const
    {
        boost::asio::ip::tcp::endpoint remote_ep = socket_.remote_endpoint();
        return remote_ep.port();
    }

    boost::uuids::uuid _accountGUID;
    std::string _login;
    uint8_t _expversion = 0;
    std::optional<Crypto::SRP6> _srp6;
    SessionKey _sessionKey = {};
    std::array<uint8_t, 16> _reconnectProof = {};
    std::optional<std::vector<uint8_t>> _totpSecret;
    LogonChallenge _logonChallenge;
    AccountTypes _securityLevel = AccountTypes::SEC_PLAYER;

private:
    void do_read();

    void process_read_buffer();

    void do_write();

    void do_send_packet(const Packet &packet);

    boost::asio::ip::tcp::socket socket_;
    std::shared_ptr<AuthServer> server_;

    MessageBuffer read_buffer_;

    std::deque<std::vector<uint8_t>> write_queue_;
    bool writing_ = false;
    std::atomic<bool> closed_{false};

    SessionMode session_mode_;
};
