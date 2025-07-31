#include "RealmList.hpp"
#include "src/authserver/AuthServer.hpp"

RealmList::RealmList(boost::asio::io_context &io_context,
                     std::shared_ptr<AuthServer> server,
                     std::chrono::seconds update_interval)
        : io_context_(io_context),
          update_timer_(io_context),
          strand_(boost::asio::make_strand(io_context)),
          resolver_(std::make_unique<Resolver>(io_context)),
          server_(std::move(server)),
          update_interval_(update_interval) {
}

RealmList::~RealmList() {
    stop();
}

void RealmList::start() {
    start_update_timer();
}

void RealmList::stop() {
    boost::system::error_code ec;
    update_timer_.cancel(ec);
}

void RealmList::load_realms(bool isFirst) {
    auto log = Logger::get();
    try {
        auto stmt = PreparedStatement("SELECT_REALMLIST");
        auto rows = server_->db()->execute_sync_many<RealmRow>(stmt);

        std::unordered_map<uint32_t, std::shared_ptr<Realm>> new_realms;

        for (const auto &row: rows) {
            auto externalAddress = resolver_->Resolve(boost::asio::ip::tcp::v4(), row.address, "");
            if (!externalAddress) {
                log->error("RealmList: Could not resolve address {} for realm \"{}\" id {}", row.address, row.name, row.id);
                continue;
            }

            auto localAddress = resolver_->Resolve(boost::asio::ip::tcp::v4(), row.localAddress, "");
            if (!localAddress) {
                log->error("RealmList: Could not resolve localAddress {} for realm \"{}\" id {}", row.localAddress, row.name, row.id);
                continue;
            }

            auto localSubmask = resolver_->Resolve(boost::asio::ip::tcp::v4(), row.localSubnetMask, "");
            if (!localSubmask) {
                log->error("RealmList: Could not resolve localSubnetMask {} for realm \"{}\" id {}", row.localSubnetMask, row.name, row.id);
                continue;
            }

            auto realm = std::make_shared<Realm>();
            realm->Id = row.id;
            realm->Name = row.name;
            realm->Build = row.build;
            realm->Type = row.icon;
            realm->Flags = static_cast<RealmFlags>(row.flag);
            realm->Timezone = row.timezone;
            realm->AllowedSecurityLevel = static_cast<AccountTypes>(row.securityLevel);
            realm->PopulationLevel = row.population;
            realm->Port = row.port;

            realm->ExternalAddress = std::make_unique<boost::asio::ip::address>(externalAddress->address());
            realm->LocalAddress = std::make_unique<boost::asio::ip::address>(localAddress->address());
            realm->LocalSubnetMask = std::make_unique<boost::asio::ip::address>(localSubmask->address());

            new_realms[row.id] = realm;
        }

        {
            std::lock_guard lock(mutex_);
            realms_.swap(new_realms);
        }

        if (isFirst)
            log->info(">>> RealmList: loaded {} realms", realms_.size());
        else
            log->trace("RealmList: Updated, {} realms loaded", realms_.size());

    } catch (const std::exception &ex) {
        log->error("RealmList::update failed: {}", ex.what());
    }
}

boost::asio::awaitable<void> RealmList::update() {
    load_realms();
    co_return;
}

std::shared_ptr<Realm> RealmList::get(uint32_t id) const {
    std::lock_guard lock(mutex_);
    auto it = realms_.find(id);
    return it != realms_.end() ? it->second : nullptr;
}

void RealmList::start_update_timer() {
    update_timer_.expires_after(update_interval_);
    update_timer_.async_wait([self = shared_from_this()](const boost::system::error_code &ec) {
        if (!ec) {
            boost::asio::co_spawn(
                    self->strand_,
                    [self]() -> boost::asio::awaitable<void> {
                        co_await self->update();
                        co_return;
                    },
                    boost::asio::detached
            );
            self->start_update_timer();
        }
    });
}
