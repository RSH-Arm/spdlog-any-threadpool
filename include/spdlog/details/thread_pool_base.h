#pragma once

#include <spdlog/common.h>
#include <spdlog/details/log_msg.h>
#include <memory>

namespace spdlog {
class async_logger;

namespace details {

using async_logger_ptr = std::shared_ptr<spdlog::async_logger>;

// Abstract interface for thread pool
// Allows users to provide their own thread pool implementation
class SPDLOG_API thread_pool_base {
public:
    virtual ~thread_pool_base() = default;

    // Post a log message to the thread pool
    virtual void post_log(async_logger_ptr&& worker_ptr,
                          const details::log_msg& msg,
                          async_overflow_policy overflow_policy) = 0;

    // Post a flush request to the thread pool
    virtual void post_flush(async_logger_ptr&& worker_ptr,
                            async_overflow_policy overflow_policy) = 0;
};

}  // namespace details
}  // namespace spdlog