#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <utility>
#include <vector>
#include <deque>
#include <atomic>

#include "utils/Duration.hpp"
#include "ObjectGuid/ObjectGuid.hpp"
#include "Cryptography/AuthCrypt.hpp"
#include "packet/MessageBuffer.hpp"
#include "packet/WoWPacket.hpp"

#include "src/relayserver/RelayServer.hpp"
#include "src/relayserver/Entity/Addons/Addons.hpp"
#include "src/relayserver/enums/AccountDataType.hpp"
#include "src/relayserver/enums/TutorialsFlag.hpp"

class RelayServer; // forward declaration

#define GLOBAL_CACHE_MASK           0x15
#define PER_CHARACTER_CACHE_MASK    0xEA

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
    std::shared_ptr<RelayServer> server() const { return server_.lock(); }
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

    /// Auth section
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

    /// Account section
    Account* getAccount() { return &account_; }
    void setAccount(Account acc) { account_ = std::move(acc); }
    TimePoint getLastPingTime() const { return m_lastPingTime; }
    void setLastPingTime(TimePoint value) { m_lastPingTime = value; }
    uint32_t getOverSpeedPings() const { return m_overSpeedPings; }
    void setOverSpeedPings(uint32_t overSpeedPings) { m_overSpeedPings = overSpeedPings; }
    uint32_t getLatency() const { return m_latency; }
    void setLatency(uint32_t latency) { m_latency = latency; }
    LocaleConstant getSessionDbcLocale() const { return m_sessionDbcLocale; }
    void setSessionDbcLocale(LocaleConstant value) { m_sessionDbcLocale = value; }

    /// Character section
    ObjectGuid getCurrentPlayerObjectGuid() const { return currentPlayerObjectGuid_; }
    void setCurrentPlayerObjectGuid(ObjectGuid value) { currentPlayerObjectGuid_ = value; }

    void resetLegitCharacterForAccount() { legitCharacters_.clear(); }
    void addLegitCharacterForAccount(ObjectGuid guid, CharacterEnumRow character) {
        legitCharacters_[guid] = std::move(character);
    }
    uint8_t getCharactersCountOnRealm() const { return legitCharacters_.size(); }
    CharacterEnumRow const* getCharacter(ObjectGuid guid) {
        auto itr = legitCharacters_.find(guid);
        if (itr != legitCharacters_.end()) {
            return &itr->second;
        }
        return nullptr;
    }

    /// DK section
    void addDKCountOnRealm() { dkCount_++; }
    uint8_t getDKCountOnRealm() const { return dkCount_; }
    void setIsCanCreateDK(bool value) { m_isCanCreateDK = value; }
    bool isCanCreateDK() const { return m_isCanCreateDK; }

    /// Account Team
    Team getAccountTeam() const { return accountTeam_; }
    void setAccountTeam(Team value) { accountTeam_ = value; }

    /// Account Data
    AccountData* getAccountData(AccountDataType type) { return &m_accountData[type]; }
    void sendAccountDataTimes(uint32_t mask);
    boost::asio::awaitable<void> setAccountData(AccountDataType type, time_t tm, std::string const& data);
    boost::asio::awaitable<void> loadAccountData(uint32_t mask, ObjectGuid characterGUID);

    /// Account cache
    void sendClientCacheVersion();

    /// Account Addons
    void readAddonsInfo(const std::vector<uint8_t>& data);
    void sendAddonsInfo();

    /// Tutorials
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

private:
    void send_auth_challenge();
    void do_read();

    void process_read_buffer();
    void do_write();

    void do_send_packet(const WoWPacket &packet);

    void schedule_processing();
    void process_packets();

    boost::asio::ip::tcp::socket socket_;
    std::weak_ptr<RelayServer> server_;
    MessageBuffer read_buffer_;

    std::deque<std::vector<uint8_t>> write_queue_;
    bool writing_ = false;
    std::deque<std::shared_ptr<WoWPacket>> recv_queue_;
    bool processing_queue_ = false;
    std::atomic<bool> closed_{false};

    /// Auth section
    AuthCrypt authCrypt_;
    bool authed_ = false;
    std::array<uint8_t, 4> authSeed_;
    std::array<uint8_t, 4> clientSeed_;

    /// Account section
    Account account_;
    TimePoint m_lastPingTime;
    uint32_t m_overSpeedPings;
    uint32_t m_latency;
    LocaleConstant m_sessionDbcLocale;

    /// Character section
    ObjectGuid currentPlayerObjectGuid_;
    std::unordered_map<ObjectGuid, CharacterEnumRow> legitCharacters_;

    /// DK section
    bool m_isCanCreateDK = false;
    uint8_t dkCount_ = 0;

    /// Account Team
    Team accountTeam_ = Team::TEAM_OTHER;

    /// Account Data
    AccountData m_accountData[NUM_ACCOUNT_DATA_TYPES];

    /// Account Addons
    Addons _addons;

    /// Tutorials
    uint32_t m_Tutorials[MAX_ACCOUNT_TUTORIAL_VALUES];
    uint8_t  m_TutorialsChanged;
};
