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
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include "spdlog/pattern_formatter.h"

class MDC {
public:
    void put(const std::string& key, const std::string& value) {
        data_[key] = value;
    }
    void clear() { data_.clear(); }
    [[nodiscard]] const std::unordered_map<std::string, std::string>& data() const { return data_; }

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

    static void shutdown() { spdlog::shutdown(); }

    static std::shared_ptr<Logger> get() {
        static std::shared_ptr<Logger> instance = std::shared_ptr<Logger>(new Logger());
        return instance;
    }

    // --- MDC wrappers ---
    void trace_with_mdc(const std::string& message, const MDC& mdc) { log_with_mdc(spdlog::level::trace, message, mdc); }
    void debug_with_mdc(const std::string& message, const MDC& mdc) { log_with_mdc(spdlog::level::debug, message, mdc); }
    void info_with_mdc(const std::string& message, const MDC& mdc) { log_with_mdc(spdlog::level::info, message, mdc); }
    void warn_with_mdc(const std::string& message, const MDC& mdc) { log_with_mdc(spdlog::level::warn, message, mdc); }
    void error_with_mdc(const std::string& message, const MDC& mdc) { log_with_mdc(spdlog::level::err, message, mdc); }
    void critical_with_mdc(const std::string& message, const MDC& mdc) { log_with_mdc(spdlog::level::critical, message, mdc); }

    // --- Regular log methods ---
    template<typename... Args>
    void trace(fmt::format_string<Args...> fmt_str, Args&&... args) { log_generic(spdlog::level::trace, fmt_str, std::forward<Args>(args)...); }
    template<typename... Args>
    void debug(fmt::format_string<Args...> fmt_str, Args&&... args) { log_generic(spdlog::level::debug, fmt_str, std::forward<Args>(args)...); }
    template<typename... Args>
    void info(fmt::format_string<Args...> fmt_str, Args&&... args) { log_generic(spdlog::level::info, fmt_str, std::forward<Args>(args)...); }
    template<typename... Args>
    void warn(fmt::format_string<Args...> fmt_str, Args&&... args) { log_generic(spdlog::level::warn, fmt_str, std::forward<Args>(args)...); }
    template<typename... Args>
    void error(fmt::format_string<Args...> fmt_str, Args&&... args) { log_generic(spdlog::level::err, fmt_str, std::forward<Args>(args)...); }
    template<typename... Args>
    void critical(fmt::format_string<Args...> fmt_str, Args&&... args) { log_generic(spdlog::level::critical, fmt_str, std::forward<Args>(args)...); }

private:
    Logger() {
        try {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::debug);

            const char* env = std::getenv("LOG_FORMAT_JSON");
            use_json_ = env && (std::string(env) == "1" || std::string(env) == "true" || std::string(env) == "yes");

            if (!use_json_) {
                console_sink->set_formatter(std::make_unique<spdlog::pattern_formatter>("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] %v"));
            } else {
                console_sink->set_formatter(std::make_unique<JsonFormatter>());
            }

            std::vector<spdlog::sink_ptr> sinks{ console_sink };

            if (const char* env_log_file = std::getenv("LOG_FILE"); env_log_file) {
                std::string val(env_log_file);
                std::transform(val.begin(), val.end(), val.begin(), ::tolower);
                if (val == "true" || val == "1" || val == "yes") {
                    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/authserver.log", 1024 * 1024 * 5, 3);
                    file_sink->set_level(spdlog::level::debug);
                    if (use_json_) file_sink->set_formatter(std::make_unique<JsonFormatter>());
                    else file_sink->set_formatter(std::make_unique<spdlog::pattern_formatter>("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v"));
                    sinks.push_back(file_sink);
                }
            }

            logger_ = std::make_shared<spdlog::async_logger>("async_logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            logger_->set_level(spdlog::level::debug);
            spdlog::register_logger(logger_);
            spdlog::set_default_logger(logger_);

        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Logger init failed: " << ex.what() << std::endl;
            throw;
        }
    }

    template<typename... Args>
    void log_generic(spdlog::level::level_enum lvl, fmt::format_string<Args...> fmt_str, Args&&... args) {
        if (use_json_) {
            // старый JSON вариант
            std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);
            logger_->log(lvl, "{{\"message\":\"{}\"}}", escape(message));
        } else {
            // plain вывод
            logger_->log(lvl, fmt_str, std::forward<Args>(args)...);
        }
    }

    void log_with_mdc(spdlog::level::level_enum lvl, const std::string& message, const MDC& mdc) {
        if (use_json_) {
            std::string msg = fmt::format("\"{}\",\"mdc\":{{{}}}", escape(message), mdc_to_json(mdc));
            logger_->log(lvl, "{{{}}}", msg);
        } else {
            if (mdc.data().empty())
                logger_->log(lvl, "{}", message);
            else {
                std::string mdc_str;
                for (const auto& [k, v] : mdc.data()) {
                    if (!mdc_str.empty()) mdc_str += ", ";
                    mdc_str += fmt::format("{}={}", k, v);
                }
                logger_->log(lvl, "{} [{}]", message, mdc_str);
            }
        }
    }

    static std::string mdc_to_json(const MDC& mdc) {
        std::string out;
        bool first = true;
        for (const auto& [k, v] : mdc.data()) {
            if (!first) out += ",";
            out += fmt::format("\"{}\":\"{}\"", escape(k), escape(v));
            first = false;
        }
        return out;
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

    // JSON форматтер
    class JsonFormatter : public spdlog::formatter {
    public:
        void format(const spdlog::details::log_msg& msg, spdlog::memory_buf_t& dest) override {
            fmt::format_to(std::back_inserter(dest), "{{{}}}\n", fmt::string_view(msg.payload.data(), msg.payload.size()));
        }
        std::unique_ptr<spdlog::formatter> clone() const override { return std::make_unique<JsonFormatter>(); }
    };

private:
    std::shared_ptr<spdlog::logger> logger_;
    bool use_json_ = false;
};
