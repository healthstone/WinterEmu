cmake_minimum_required(VERSION 3.11)
project(WinterEmu)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# Добавляем Catch2 через FetchContent
find_package(Catch2 REQUIRED)

# spdlog
set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/deps/spdlog-1.15.3)
add_subdirectory(${SPDLOG_DIR})

file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS src/common/*.cpp)
file(GLOB_RECURSE AUTH_SERVER_SOURCES CONFIGURE_DEPENDS src/authserver/*.cpp)
file(GLOB_RECURSE WORLD_SERVER_SOURCES CONFIGURE_DEPENDS src/worldserver/*.cpp)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)

set(COMMON_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/common
        ${CMAKE_SOURCE_DIR}/src/common/database
)

# === auth Сервер ===
add_executable(authserver
        src/main_authserver.cpp
        ${AUTH_SERVER_SOURCES}
        ${COMMON_SOURCES}
)

target_link_libraries(authserver PRIVATE
        Boost::boost Boost::system
        ${PostgreSQL_LIBRARIES}
        ${PQXX_LIBRARIES}
        spdlog::spdlog
        OpenSSL::Crypto
)

target_include_directories(authserver PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${PostgreSQL_INCLUDE_DIRS}
        ${PQXX_INCLUDE_DIRS}
)

# === auth Сервер ===
add_executable(worldserver
        src/main_worldserver.cpp
        ${WORLD_SERVER_SOURCES}
        ${COMMON_SOURCES}
)

target_link_libraries(worldserver PRIVATE
        Boost::boost Boost::system
        ${PostgreSQL_LIBRARIES}
        ${PQXX_LIBRARIES}
        spdlog::spdlog
        OpenSSL::Crypto
)

target_include_directories(worldserver PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${PostgreSQL_INCLUDE_DIRS}
        ${PQXX_INCLUDE_DIRS}
)

# === Тесты ===
add_executable(tests
        tests/tests_main.cpp
        ${TEST_SOURCES}
        ${COMMON_SOURCES}
)

target_link_libraries(tests PRIVATE
        Boost::boost Boost::system
        Catch2::Catch2WithMain
        OpenSSL::Crypto
        spdlog::spdlog
)

target_include_directories(tests PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
)