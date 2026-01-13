cmake_minimum_required(VERSION 3.11)
project(WinterEmu LANGUAGES CXX)

# =========================
# Общие настройки
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

# =========================
# Зависимости
# =========================
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

find_package(Boost REQUIRED COMPONENTS
        system
        locale
        stacktrace_backtrace
        regex
)

find_package(PostgreSQL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# =========================
# Catch2 (tests)
# =========================
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

# =========================
# spdlog
# =========================
set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/deps/spdlog-1.15.3)
add_subdirectory(${SPDLOG_DIR})

# =========================
# utf8cpp
# =========================
set(UTF8CPP_DIR ${CMAKE_SOURCE_DIR}/deps/utf8cpp)
add_subdirectory(${UTF8CPP_DIR})

# =========================
# Исходники
# =========================
file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS
        src/common/*.cpp
)

file(GLOB_RECURSE GAME_COMMON_SOURCES CONFIGURE_DEPENDS
        src/game/*.cpp
)

file(GLOB_RECURSE AUTH_SERVER_SOURCES CONFIGURE_DEPENDS
        src/authserver/*.cpp
)

file(GLOB_RECURSE RELAY_SERVER_SOURCES CONFIGURE_DEPENDS
        src/relayserver/*.cpp
)

file(GLOB_RECURSE NODE_SERVER_SOURCES CONFIGURE_DEPENDS
        src/nodeserver/*.cpp
)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        tests/*.cpp
)

# =========================
# Include dirs
# =========================
set(COMMON_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/common
        ${CMAKE_SOURCE_DIR}/src/common/database
        ${CMAKE_SOURCE_DIR}
)

# =========================
# Общая библиотека
# =========================
add_library(winteremu_common STATIC
        ${COMMON_SOURCES}
        ${GAME_COMMON_SOURCES}
)

target_include_directories(winteremu_common PUBLIC
        ${COMMON_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${PostgreSQL_INCLUDE_DIRS}
        ${PQXX_INCLUDE_DIRS}
)

target_link_libraries(winteremu_common PUBLIC
        Boost::system
        Boost::locale
        Boost::stacktrace_backtrace
        Boost::regex
        OpenSSL::Crypto
        ${PostgreSQL_LIBRARIES}
        ${PQXX_LIBRARIES}
        spdlog::spdlog
)

# =========================
# authserver
# =========================
add_executable(authserver
        src/main_authserver.cpp
        ${AUTH_SERVER_SOURCES}
)

target_link_libraries(authserver PRIVATE
        winteremu_common
)

# =========================
# relayserver
# =========================
add_executable(relayserver
        src/main_relayserver.cpp
        ${RELAY_SERVER_SOURCES}
)

target_link_libraries(relayserver PRIVATE
        winteremu_common
        ZLIB::ZLIB
)

# =========================
# nodeserver
# =========================
add_executable(nodeserver
        src/main_nodeserver.cpp
        ${NODE_SERVER_SOURCES}
)

target_link_libraries(nodeserver PRIVATE
        winteremu_common
)

# =========================
# tests
# =========================
add_executable(tests
        tests/tests_main.cpp
        ${TEST_SOURCES}
)

target_link_libraries(tests PRIVATE
        winteremu_common
        Catch2::Catch2WithMain
)

# =========================
# (опционально) CTest
# =========================
enable_testing()
