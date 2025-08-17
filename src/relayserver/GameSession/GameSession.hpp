#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <deque>
#include <atomic>

#include "src/relayserver/RelayServer.hpp"
#include "packet/MessageBuffer.hpp"
#include "packet/WoWPacket.hpp"
#include "Cryptography/AuthCrypt.hpp"
#include "utils/Duration.hpp"
#include "src/relayserver/enums/AccountDataType.hpp"

class RelayServer; // forward declaration

#define NUM_ACCOUNT_DATA_TYPES        8

#define GLOBAL_CACHE_MASK           0x15
struct AccountData
{
    AccountData() : Time(0), Data("") { }

    time_t Time;
    std::string Data;
};

class GameSession : public std::enable_shared_from_this<GameSession> {
public:
    GameSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<RelayServer> server);

    void start();

    void close();

    bool isOpened() const { return !closed_; }

    void send_packet(const std::shared_ptr<const WoWPacket>& packet);

    boost::asio::ip::tcp::socket &socket() { return socket_; }

    std::shared_ptr<RelayServer> server() const { return server_; }

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

    void initCrypt(const std::array<uint8_t, 40>& key);
    void setAuthed(bool authed) { authed_ = authed; }
    bool isAuthed() const { return authed_; }

    const std::array<uint8_t, 4>& authSeed() const {
        return authSeed_;
    }

    void setAuthSeed(const std::array<uint8_t, 4>& seed) {
        authSeed_ = seed;
    }

    void setClientSeed(std::array<uint8_t, 4> seed) { clientSeed_ = seed; }
    std::array<uint8_t, 4> clientSeed() const { return clientSeed_; }

    // GameSection
    uint32_t getLatency() const { return m_latency; }
    void setLatency(uint32_t latency) { m_latency = latency; }
    bool handlePing(const std::shared_ptr<WoWPacket>& p);


    boost::uuids::uuid const& getAccountId() const { return accountId_; }
    void setAccountId(boost::uuids::uuid const &value) { accountId_ = value; }
    std::string const& getAccountName() const { return accountName_; }
    void setAccountName(std::string const &value) { accountName_= value; }

    boost::uuids::uuid const& getCharacterId() const { return characterId_; }
    //void setCharacterId(boost::uuids::uuid const &value) { characterId_ = value; }
    std::string const& getCharacterName() const { return characterName_; }
    void setCharacterName(std::string const &value) { characterName_= value; }

    // Account Data
    AccountData* getAccountData(AccountDataType type) { return &m_accountData[type]; }
    void sendAccountDataTimes(uint32_t mask);
    boost::asio::awaitable<void> setAccountData(AccountDataType type, time_t tm, std::string const& data);
    //void LoadAccountData(PreparedQueryResult result, uint32 mask);

private:
    void send_auth_challenge();

    void do_read();

    void process_read_buffer();

    void do_write();

    void do_send_packet(const WoWPacket &packet);

    boost::asio::ip::tcp::socket socket_;
    std::shared_ptr<RelayServer> server_;

    MessageBuffer read_buffer_;

    std::deque<std::vector<uint8_t>> write_queue_;
    bool writing_ = false;
    std::atomic<bool> closed_{false};

    AuthCrypt authCrypt_;
    bool authed_ = false;
    std::array<uint8_t, 4> authSeed_;
    std::array<uint8_t, 4> clientSeed_;

    // GameSection
    boost::uuids::uuid accountId_;
    std::string accountName_;

    boost::uuids::uuid characterId_;
    std::string characterName_;

    TimePoint lastPingTime_;
    uint32_t overSpeedPings_;
    uint32_t m_latency;

    AccountData m_accountData[NUM_ACCOUNT_DATA_TYPES];
};
