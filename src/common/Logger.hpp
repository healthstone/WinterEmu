#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <sstream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>           // НЕ цветной консольный sink
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <fmt/format.h>

class MDC {
public:
    void put(const std::string& key, const std::string& value) {
        data_[key] = value;
    }

    void clear() {
        data_.clear();
    }

    [[nodiscard]] const std::unordered_map<std::string, std::string>& data() const {
        return data_;
    }

private:
    std::unordered_map<std::string, std::string> data_;
};

class Logger : public std::enable_shared_from_this<Logger> {
public:
    static void init_thread_pool() {
        constexpr size_t queue_size = 8192;
        constexpr size_t num_threads = 1;
        spdlog::init_thread_pool(queue_size, num_threads);
    }

    static void shutdown() {
        spdlog::shutdown();
    }

    static std::shared_ptr<Logger> get() {
        static std::shared_ptr<Logger> instance = std::shared_ptr<Logger>(new Logger());
        return instance;
    }

    // --- MDC wrappers ---
    void trace_with_mdc(const std::string& message, const MDC& mdc) {
        logger_->trace("{}", format_with_mdc(message, mdc));
    }

    void debug_with_mdc(const std::string& message, const MDC& mdc) {
        logger_->debug("{}", format_with_mdc(message, mdc));
    }

    void info_with_mdc(const std::string& message, const MDC& mdc) {
        logger_->info("{}", format_with_mdc(message, mdc));
    }

    void warn_with_mdc(const std::string& message, const MDC& mdc) {
        logger_->warn("{}", format_with_mdc(message, mdc));
    }

    void error_with_mdc(const std::string& message, const MDC& mdc) {
        logger_->error("{}", format_with_mdc(message, mdc));
    }

    void critical_with_mdc(const std::string& message, const MDC& mdc) {
        logger_->critical("{}", format_with_mdc(message, mdc));
    }

    // --- Regular log methods with fmt::format_string ---
    template<typename... Args>
    void trace(fmt::format_string<Args...> fmt, Args&&... args) {
        log_json(spdlog::level::trace, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(fmt::format_string<Args...> fmt, Args&&... args) {
        log_json(spdlog::level::debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(fmt::format_string<Args...> fmt, Args&&... args) {
        log_json(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(fmt::format_string<Args...> fmt, Args&&... args) {
        log_json(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(fmt::format_string<Args...> fmt, Args&&... args) {
        log_json(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(fmt::format_string<Args...> fmt, Args&&... args) {
        log_json(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }

private:
    Logger() {
        try {
            // НЕ цветной консольный sink
            auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
            console_sink->set_level(spdlog::level::debug);
            console_sink->set_formatter(std::make_unique<JsonFormatter>());

            std::vector<spdlog::sink_ptr> sinks{ console_sink };

            const char* env_log_file = std::getenv("LOG_FILE");
            bool enable_file_log = false;
            if (env_log_file) {
                std::string val(env_log_file);
                std::transform(val.begin(), val.end(), val.begin(), ::tolower);
                if (val == "true" || val == "1" || val == "yes") {
                    enable_file_log = true;
                }
            }

            if (enable_file_log) {
                auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                        "logs/authserver.log", 1024 * 1024 * 5, 3);
                file_sink->set_level(spdlog::level::debug);
                file_sink->set_formatter(std::make_unique<JsonFormatter>());
                sinks.push_back(file_sink);
            }

            logger_ = std::make_shared<spdlog::async_logger>(
                    "async_logger",
                    sinks.begin(),
                    sinks.end(),
                    spdlog::thread_pool(),
                    spdlog::async_overflow_policy::block
            );

            logger_->set_level(spdlog::level::debug);
            spdlog::register_logger(logger_);
            spdlog::set_default_logger(logger_);

        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Logger init failed: " << ex.what() << std::endl;
            throw;
        }
    }

    template<typename... Args>
    void log_json(spdlog::level::level_enum lvl, fmt::format_string<Args...> fmt_str, Args&&... args) {
        std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);
        std::ostringstream oss;
        oss << "\"message\":\"" << escape(message) << "\"";
        logger_->log(lvl, "{}", oss.str());
    }

    std::string format_with_mdc(const std::string& message, const MDC& mdc) {
        std::ostringstream oss;
        oss << "\"message\":\"" << escape(message) << "\",\"mdc\":" << mdc_to_json(mdc);
        return oss.str();
    }

    static std::string mdc_to_json(const MDC& mdc) {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto& [k, v] : mdc.data()) {
            if (!first) oss << ",";
            oss << "\"" << escape(k) << "\":\"" << escape(v) << "\"";
            first = false;
        }
        oss << "}";
        return oss.str();
    }

    static std::string escape(const std::string& s) {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    }

    // JSON форматтер без цвета (консоль и файл)
    class JsonFormatter : public spdlog::formatter {
    public:
        void format(const spdlog::details::log_msg& msg, spdlog::memory_buf_t& dest) override {
            using namespace std::chrono;

            auto tp = msg.time;
            auto s = time_point_cast<seconds>(tp);
            auto ms = time_point_cast<milliseconds>(tp) - time_point_cast<milliseconds>(s);
            auto ns = time_point_cast<nanoseconds>(tp) - time_point_cast<nanoseconds>(time_point_cast<milliseconds>(tp));

            auto time_t = system_clock::to_time_t(s);
            std::tm tm;
#ifdef _WIN32
            localtime_s(&tm, &time_t);
#else
            localtime_r(&time_t, &tm);
#endif

            char time_buf[64];
            std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%dT%H:%M:%S", &tm);

            // msg.payload уже содержит JSON без внешних скобок, вставляем "как есть"
            fmt::format_to(
                    std::back_inserter(dest),
                    "{{\"timestamp\":\"{}.{:03}{:03}\",\"level\":\"{}\",\"thread_id\":{},{} }}\n",
                    time_buf,
                    static_cast<int>(ms.count()),
                    static_cast<int>(ns.count()),
                    spdlog::level::to_string_view(msg.level),
                    msg.thread_id,
                    fmt::string_view(msg.payload.data(), msg.payload.size())
            );
        }

        std::unique_ptr<spdlog::formatter> clone() const override {
            return std::make_unique<JsonFormatter>();
        }
    };

private:
    std::shared_ptr<spdlog::logger> logger_;
};
