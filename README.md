# Purity Server: Overview

This C++ project implements a **Asynchronous server** using **Boost.Asio**, a custom binary protocol with **MMORPG standart** packet framing, SRP (Secure Remote Password) for authentication logic, and PostgreSQL integration.

## Key Components

### 🧩 **Session Modes**

The server supports **multi-stage session modes** to switch between authentication and work phases:

- **AUTH_SESSION**
    - Packet format: `[Opcode (1 byte)][Length (2 bytes BE)][Payload]`
    - Used for authentication handshake (SRP).
    - Example opcodes: `CMSG_AUTH_LOGON_CHALLENGE`, `CMSG_PING`.

- **WORK_SESSION**
    - Packet format: `[Opcode (2 bytes BE)][Length (2 bytes BE)][Payload]`
    - Used for post-auth gameplay or chat.
    - Example opcodes: `CMSG_PING`, `CMSG_MESSAGE`.

**Note:**
- `BE` = Big Endian for network consistency.
- Payloads are binary buffers encoded with a custom `ByteBuffer`.
- The **Client** auto-switches session mode (e.g., from `AUTH_SESSION` to `WORK_SESSION`) after receiving `SMSG_AUTH_LOGON_PROOF`.

---

- For each SessionMode you can create child of Packet with new structure. All methods (read_uint8, write_uint8 and etc... ) already exist and tested by ByteBufferTest

### ⚙️ **Packet Handlers**

- `HandlersAuth` and `HandlersWork` dispatch packets by opcode.
- All packets have an async `dispatch` system for routing.
- For DB operations (e.g., account lookups), blocking calls run safely in worker threads, responses marshal back to the I/O thread.

### ✅ **ClientSession** (`ClientSession.cpp`)
- Owns a TCP socket and buffers.
- Fully **thread-safe**: `send_packet()` posts safely back to the I/O thread.
- Reads framed packets using handlers and reader from session mode.
- Uses a `MessageBuffer` for incremental reading.
- Manages its own lifetime via `shared_from_this()`.

### ✅ **Server** (`Server.cpp`)
- Accepts new connections asynchronously.
- Manages active sessions with a `std::mutex`.
- Cleanly stops all sessions and the DB pool on shutdown.

### ✅ **SRP (Secure Remote Password)** (`SRP.cpp`)
- Implements RFC 5054 style ephemeral key generation and proof.
- Supports fake challenges to mitigate timing attacks.
- Uses OpenSSL BIGNUMs with proper lifetime management.

### Technical Highlights

- **All packet headers (opcode and length fields) are big-endian.**
- Safe async write queue for each client.
- `Handlers` run CPU-bound DB calls in dedicated threads but serialize back replies to the main I/O thread.
- `SRP` is prepared for PvPGN-like proof-of-concept authentication.
- All socket writes are guarded against race conditions.

### Security & Stability

- All OpenSSL BIGNUMs are freed properly.
- DB pool handles broken connections.
- Sessions auto-cleanup on error or disconnect.
- No leaks in the async read/write chain.

### How to Extend

- Add new **opcodes** in `HandlersAuth` or `HandlersWork`.
- Add new SQL prepared statements in your DB class.
- Extend `SRP` for real password verification.
- Add new session modes if needed (copy `AUTH_SESSION` structure).

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

### Database example:

Table **accounts** with example account: login: **1**, pass: **1**

```
CREATE TABLE accounts (
  id BIGSERIAL PRIMARY KEY,
  username VARCHAR(50) UNIQUE NOT NULL,
  salt BYTEA NOT NULL CHECK (octet_length(salt) = 32),
  verifier BYTEA NOT NULL CHECK (octet_length(verifier) = 32),
  email VARCHAR(100),
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO accounts (id, username, salt, verifier, email, created_at) VALUES
(
  1,
  '1',
  decode('0A3E57EEA85D222817B72F5A6299B642D56DB174F522FF0F8C72172D1223AE63', 'hex'),
  decode('D08A4D3949264CFA1536E41EE8E0FCA3000D07B8BF3DCC1D524AA10535723689', 'hex'),
  'test@gmail.com',
  '2025-05-01 23:11:58'
);
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

### ⚡️Compilation

mkdir build && cd build

cmake ..

make -j 4             (or another threads count)

---
