#include "../include/AsioHttpClient.hpp"
#include "../include/AsioExecutionContext.hpp"
#include <asio.hpp>
#include <network/uri.hpp>
#include <api/ExecutionContext.hpp>
#include <api/HttpUrlConnection.hpp>

using namespace ledger::core;

using asio::ip::tcp;


AsioHttpClient::AsioHttpClient(const std::shared_ptr<AsioExecutionContext> &context) {
    _context = context;
}

void AsioHttpClient::execute(const std::shared_ptr<api::HttpRequest> &request) {
    struct Query {
        asio::streambuf request;
        asio::streambuf response;
        asio::ip::tcp::resolver resolver;
        asio::ip::tcp::socket socket;

        std::shared_ptr<ledger::core::api::HttpRequest> apiRequest;

        Query(asio::io_service& io_service, const std::shared_ptr<api::HttpRequest> &request)
        : resolver(io_service), socket(io_service), apiRequest(request) {

        }
    };

    
}
