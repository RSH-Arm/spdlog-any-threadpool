// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/logger.h>
#include <spdlog/details/thread_pool_base.h>

namespace spdlog {

namespace details {
class thread_pool_base;
class thread_pool;  // forward declaration for backward compatibility
}  // namespace details

class SPDLOG_API async_logger final : public std::enable_shared_from_this<async_logger>,
                                      public logger {
public:
    template <typename It>
    async_logger(std::string logger_name,
                 It begin,
                 It end,
                 std::weak_ptr<details::thread_pool_base> tp,
                 async_overflow_policy overflow_policy = async_overflow_policy::block)
        : logger(std::move(logger_name), begin, end),
          thread_pool_(std::move(tp)),
          overflow_policy_(overflow_policy) {}

    async_logger(std::string logger_name,
                 sinks_init_list sinks_list,
                 std::weak_ptr<details::thread_pool_base> tp,
                 async_overflow_policy overflow_policy = async_overflow_policy::block);

    async_logger(std::string logger_name,
                 sink_ptr single_sink,
                 std::weak_ptr<details::thread_pool_base> tp,
                 async_overflow_policy overflow_policy = async_overflow_policy::block);

    void process_backend_log(const details::log_msg &msg) { backend_sink_it_(msg); }

    void process_backend_flush() { backend_flush_(); }

    std::shared_ptr<logger> clone(std::string new_name) override;

protected:
    void sink_it_(const details::log_msg &msg) override;
    void flush_() override;
    void backend_sink_it_(const details::log_msg &incoming_log_msg);
    void backend_flush_();

private:
    std::weak_ptr<details::thread_pool_base> thread_pool_;
    async_overflow_policy overflow_policy_;

    // Дружественные классы для доступа к backend_* методам
    friend class details::thread_pool_base;
    friend class details::thread_pool;
};

}  // namespace spdlog

#ifdef SPDLOG_HEADER_ONLY
#include "async_logger-inl.h"
#endif