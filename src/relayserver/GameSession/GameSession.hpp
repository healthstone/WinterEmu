#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <utility>
#include <vector>
#include <deque>
#include <atomic>

#include "ObjectGuid/ObjectGuid.hpp"
#include "src/relayserver/RelayServer.hpp"
#include "packet/MessageBuffer.hpp"
#include "packet/WoWPacket.hpp"
#include "Cryptography/AuthCrypt.hpp"
#include "utils/Duration.hpp"
#include "src/relayserver/Entity/Addons/Addons.hpp"
#include "src/relayserver/enums/AccountDataType.hpp"
#include "src/relayserver/enums/TutorialsFlag.hpp"

class RelayServer; // forward declaration

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

    void cleanBeforeDelete();

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
    TimePoint getLastPingTime() const { return m_lastPingTime; }
    void setLastPingTime(TimePoint value) { m_lastPingTime = value; }
    uint32_t getOverSpeedPings() const { return m_overSpeedPings; }
    void setOverSpeedPings(uint32_t overSpeedPings) { m_overSpeedPings = overSpeedPings; }
    uint32_t getLatency() const { return m_latency; }
    void setLatency(uint32_t latency) { m_latency = latency; }

    ObjectGuid getObjectGuid() const { return characterGUID_; }
    bool isLegitCharacterForAccount(ObjectGuid lowGUID)
    {
        return _legitCharacters.find(lowGUID) != _legitCharacters.end();
    }
    void addLegitCharacterForAccount(ObjectGuid lowGUID) {
        _legitCharacters.insert(lowGUID);
    }
    uint8_t getCharactersCountOnRealm() const { return _legitCharacters.size(); }

    // Account Data
    AccountsRow* getAccount() { return &account_; }
    void setAccount(AccountsRow acc) { account_ = std::move(acc); }
    AccountData* getAccountData(AccountDataType type) { return &m_accountData[type]; }
    void sendAccountDataTimes(uint32_t mask);
    boost::asio::awaitable<void> setAccountData(AccountDataType type, time_t tm, std::string const& data);
    boost::asio::awaitable<void> loadAccountData(uint32_t mask);

    boost::asio::awaitable<void> loadTutorialsData();
    void sendTutorialsData();
    //void saveTutorialsData();
    uint32_t getTutorialInt(uint8_t index) const { return m_Tutorials[index]; }
    void setTutorialInt(uint8_t index, uint32_t value)
    {
        if (m_Tutorials[index] != value)
        {
            m_Tutorials[index] = value;
            m_TutorialsChanged |= TUTORIALS_FLAG_CHANGED;
        }
    }

    void setIsCanCreateDK(bool value) { m_isCanCreateDK = value; }
    bool isCanCreateDK() const { return m_isCanCreateDK; }

    void readAddonsInfo(const std::vector<uint8_t>& data);
    void sendAddonsInfo();
    void sendClientCacheVersion();

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
    AccountsRow account_;
    ObjectGuid characterGUID_;
    // this stores the GUIDs of the characters who can login
    GuidSet _legitCharacters;

    TimePoint m_lastPingTime;
    uint32_t m_overSpeedPings;
    uint32_t m_latency;

    AccountData m_accountData[NUM_ACCOUNT_DATA_TYPES];
    uint32_t m_Tutorials[MAX_ACCOUNT_TUTORIAL_VALUES];
    uint8_t  m_TutorialsChanged;
    bool m_isCanCreateDK = false;

    Addons _addons;
};
