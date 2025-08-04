
# ❄️ WinterEmu — World of Warcraft Server Emulator

**WinterEmu** is a modern World of Warcraft **server emulator** written in **C++**, using an asynchronous networking model with **Boost.Asio** and a **PostgreSQL** backend. This emulator handles account authentication, realm management, client build checks and routes traffic via a multi-threaded RelayServer with Node connections.

---

## 📌 Features

✅ Fully asynchronous `AuthServer` and `RelayServer` with `Boost.Asio`  
✅ Multi-connection `NodeConnector` pool for horizontal scaling  
✅ Clean layered architecture: `ClientSession`, `GameSession`, `NodeManager`, `NodeConnector`  
✅ Fast PostgreSQL backend with prepared statements  
✅ SRP6 authentication for secure login  
✅ Hex dump logging for all packets with `Packet::log_raw_payload`  
✅ Account and realm caching to reduce DB pressure  
✅ Supports multiple WoW builds (up to 3.3.5a)  
✅ Unit tests included

---

## ⚙️ Configuration

All main settings are configured via environment variables.

**Database settings:**

```cpp
std::string db_host = std::getenv("DB_URL") ? std::getenv("DB_URL") : "127.0.0.1";
std::string db_port = std::getenv("DB_PORT") ? std::getenv("DB_PORT") : "5432";
std::string db_user = std::getenv("DB_USER") ? std::getenv("DB_USER") : "postgres";
std::string db_password = std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "postgres";
std::string db_name = std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "postgres";
```

**AuthServer:**

```cpp
unsigned int async_threads = std::getenv("AUTH_DB_ASYNC_THREADS");
int port = std::getenv("AUTH_PORT") ? std::stoi(std::getenv("AUTH_PORT")) : 3724;
unsigned int network_threads = std::getenv("AUTH_NETWORK_THREADS") ?: 2;
```

**RelayServer:**

```cpp
unsigned int async_threads = std::getenv("RELAY_DB_ASYNC_THREADS");
int port = std::getenv("RELAY_PORT") ? std::stoi(std::getenv("RELAY_PORT")) : 8085;
unsigned int network_threads = std::getenv("RELAY_NETWORK_THREADS") ?: default: max threads - async db threads - 1;
```

The RelayServer uses `NodeManager` to open **multiple connections** to each NodeServer, matching the NodeServer's `network_threads`. This ensures parallel handling and load balancing.

Example to set up:
```bash
export DB_URL=192.168.1.100
export DB_USER=myuser
export DB_PASSWORD=mypass
export RELAY_NETWORK_THREADS=5
export RELAY_DB_ASYNC_THREADS=2
./relayserver
```

---

## 🗂️ Structure

- **AuthServer** — handles login, SRP6 handshake, proof validation
- **RelayServer** — bridges clients to world logic, handles sessions
- **NodeManager** — manages pools of `NodeConnector` connections
- **NodeConnector** — async TCP connection to NodeServer
- **GameSession** — a connected game client session
- **Packet**, **NodePacket**, **WoWPacket**, **NodeData** — binary abstractions
- **Logger** — spdlog-based async logging with hex dumps

---

## 🧩 Database

Tables:
- `accounts` (login, salt, verifier, session_key)
- `realmlist` (realms & build versions)
- `build_info` (patch info)
- `build_executable_hash` (executable checksums)

All queries are prepared for performance.

---

## 🔑 Authentication

1. Client sends `AUTH_LOGON_CHALLENGE`
2. Server verifies, loads SRP6 data
3. Salt/verifier returned or fetched from DB
4. Proof exchange and `SessionKey` generation
5. SessionKey is cached

---

## 🔁 NodeConnector Pool Example

RelayServer uses `NodeManager` to open multiple connections:

```cpp
node_manager->add_connectors(1, "127.0.0.1", 8086, 5);
```

When sending a `NodePacket`:

```cpp
auto connector = node_manager->get_first_connector(1); // picks random
connector->send_packet(packet);
```

---

## 🧪 Unit Tests

Unit tests included for SRP6, DB, packet parsing.

---

## 🛠️ Dependencies

- C++20
- Boost.Asio
- spdlog
- libpq / libpqxx
- Catch2

Install:
```bash
sudo apt install libssl-dev libboost-all-dev libpq-dev libpqxx-dev pkg-config catch2
```

---

## 🚀 Getting Started

```bash
mkdir build && cd build
cmake ..
make
./authserver
./relayserver
```

---

**WinterEmu** — control your realm. ❄️