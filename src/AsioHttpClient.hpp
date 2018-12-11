#pragma once

#include <asio.hpp>
#include <ledger/core/api/HttpClient.hpp>
#include <ledger/core/api/ExecutionContext.hpp>
#include <ledger/core/api/HttpMethod.hpp>
#include <ledger/core/api/HttpReadBodyResult.hpp>
#include <ledger/core/api/HttpUrlConnection.hpp>
#include <ledger/core/api/HttpRequest.hpp>



class AsioExecutionContext;

class AsioHttpClient : public ledger::core::api::HttpClient {
public:
    AsioHttpClient(const std::shared_ptr<AsioExecutionContext>& context);
    void execute(const std::shared_ptr<ledger::core::api::HttpRequest> &request) override;
private:
    std::shared_ptr<AsioExecutionContext> _context;
};
