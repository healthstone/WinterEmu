#pragma once

#include <pqxx/pqxx>
#include <string>

class DatabasePreparer {
public:
    static void prepare_all(pqxx::connection &conn);

private:
    static void prepareAuthSchema(pqxx::connection &conn);
    static void prepareRelaySchema(pqxx::connection &conn);
    static void prepareDBCSchema(pqxx::connection &conn);
    static void prepareWorldSchema(pqxx::connection &conn);

    // Вспомогательные методы для получения имен схем из переменных окружения
    static std::string getAuthSchema();
    static std::string getRelaySchema();
    static std::string getDBCSchema();
    static std::string getWorldSchema();
};