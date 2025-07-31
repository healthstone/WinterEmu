# ❄️ WinterEmu — World of Warcraft Auth Server Emulator

**WinterEmu** is a modern World of Warcraft **auth server emulator** (inspired by TrinityCore) written in **C++**, using an asynchronous networking model with **Boost.Asio** and a **PostgreSQL** backend. This component handles account authentication, client build verification, and realmlist delivery.

---

## 📌 Features

✅ Fully asynchronous `AuthServer` using `Boost.Asio`  
✅ Clean architecture: `ClientSession`, `RealmList`, `AccountCache`  
✅ Account and realm storage in **PostgreSQL**  
✅ SRP6 authentication protocol implemented  
✅ Full packet hex logging with `Packet::log_raw_payload`  
✅ Automatic duplicate session kick on multiple logins  
✅ Account caching with SRP6 verifier, salt, and SessionKey  
✅ Supports multiple WoW client builds up to **3.3.5a**  
✅ Unit tests are included

---

### 📦 Configuration via Environment Variables

The server automatically reads database connection settings from **environment variables**, with safe defaults if not set:

```cpp
std::string db_host = std::getenv("DB_URL") ? std::getenv("DB_URL") : "127.0.0.1";
std::string db_port = std::getenv("DB_PORT") ? std::getenv("DB_PORT") : "5432";
std::string db_user = std::getenv("DB_USER") ? std::getenv("DB_USER") : "postgres";
std::string db_password = std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "postgres";
std::string db_name = std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "postgres";
```

---

### ⚙️ Variables:
- **DB_URL** — PostgreSQL host address (default `127.0.0.1`)
- **DB_PORT** — PostgreSQL port (default `5432`)
- **DB_USER** — database user (default `postgres`)
- **DB_PASSWORD** — database password (default `postgres`)
- **DB_NAME** — database name (default `postgres`)

If an environment variable is not set, a safe fallback will be used. The log output shows exactly which values are applied.

Example for Linux:
```bash
export DB_URL=192.168.1.100
export DB_USER=myuser
export DB_PASSWORD=mypass
./authserver
```

This makes it easy to configure the server without changing the source code! ✅

---

## ⚙️ Structure

- `AuthServer` — the main server entry point, handles connections and sessions.
- `ClientSession` — represents a connected client, tracks SRP6 states, SessionKey, Proofs, and manages the WoW protocol.
- `RealmList` — keeps the current realm list from the database, resolves IP for the client.
- `AccountCache` — fast in-memory cache for account SRP6 data and SessionKeys.
- `Packet` / `RawPacket` — binary packet abstraction for WoW login protocol.

---

## 🗄️ Database

- Uses **PostgreSQL**.
- Example tables:
  - `accounts` — stores login, salt, verifier, session_key.
  - `realmlist` — stores realm IPs, build version, flags.
  - `build_info` — stores build info data.
  - `build_executable_hash` — stores build exe hash data.
- Uses prepared statements for queries.

## 🔑 How Authentication Works

1. The client sends `AUTH_LOGON_CHALLENGE` or `AUTH_RECONNECT_CHALLENGE`.
2. The server parses the packet, validates UTF-8, verifies account.
3. If the account is cached — SRP6 B, salt, and SessionKey are sent immediately.
4. Otherwise, salt/verifier are loaded from the database and cached.
5. The server responds with proof challenge, and validates proof.
6. On success, the `SessionKey` is saved back into the cache.

---

## 🧩 Dependencies

- `C++20`
- `Boost.Asio`
- `PostgreSQL client library`
- `spdlog` (for logging)
- `Catch2` or any other unit testing framework

---

### Requirements

```
sudo apt install \
libssl-dev \
libboost-all-dev \
libpq-dev \
libpqxx-dev \
pkg-config \
catch2
```

## 🚀 Getting Started

```bash
mkdir build
cd build
cmake ..
make
./authserver
```

- Make sure **PostgreSQL** is running and schema is created.
- Adjust database connection strings in your config files.

---

## 📜 Example Logs

```
[client_session][start] New connection from 127.0.0.1:33044
[Packet] DUMP opcode ID: REQUEST AUTH_LOGON_CHALLENGE ...
[Packet] DUMP opcode ID: RESPONSE AUTH_LOGON_CHALLENGE ...
```

Every raw packet is hex-dumped for debugging.

---

## ✅ Unit Testing

The project includes unit tests for critical parts such as SRP6, packet parsing, and database logic.

Framework: Catch2

---

## 🤝 Contributing

PRs and issues are welcome!

---

**WinterEmu** — Blizzard may freeze, but you control the realms. ❄️
