#pragma once
#include <ledger/core/api/ExecutionContext.hpp>
#include <asio.hpp>
#include <queue>

namespace ledger {
    namespace core {
        namespace api {
            class Runnable;
        };
    }
}

class AsioExecutionContext : public ledger::core::api::ExecutionContext {
public:
    AsioExecutionContext() : _io_service(new asio::io_service()) {};
    void execute(const std::shared_ptr<ledger::core::api::Runnable> & runnable) override;

    void delay(const std::shared_ptr<ledger::core::api::Runnable> & runnable, int64_t millis) override;

    void run();

    std::shared_ptr<asio::io_service> getIoService() { return _io_service; };
private:
    bool runOne();
private:
    std::queue<std::shared_ptr<ledger::core::api::Runnable>> q;
    std::shared_ptr<asio::io_service> _io_service;
};