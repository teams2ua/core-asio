/*
 *
 * AsioHttpClient
 * ledger-core
 *
 * Created by Pierre Pollastri on 21/03/2017.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Ledger
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <future>
#include "AsioHttpClient.hpp"
#include "AsioExecutionContext.hpp"
#include <asio.hpp>
#include <regex>
#include <network/uri.hpp>
#include <ledger/core/api/ExecutionContext.hpp>
#include <ledger/core/api/HttpUrlConnection.hpp>

using namespace ledger::core;

using asio::ip::tcp;

class RequestResponse : public std::enable_shared_from_this<RequestResponse>
{
public:
    RequestResponse(const std::shared_ptr<asio::io_context>& io_context,
        const std::string& url)
        : resolver_(*io_context),
        socket_(*io_context) {
    }

    void execute(const std::string& url) {
        network::uri uri(url);
        std::ostream request_stream(&request_);
        request_stream << "GET " << uri.path().to_string() + uri.fragment().to_string() << " HTTP/1.0\r\n";
        request_stream << "Host: " << uri.authority().to_string() << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";
        resolver_.async_resolve(
            uri.authority().to_string(),
            "http",
            [](const asio::error_code& err, const tcp::resolver::results_type& endpoints) {
            std::cout << endpoints.size() << std::endl;
        });
    }

private:
    void handle_resolve(const asio::error_code& err,
        const tcp::resolver::results_type& endpoints)
    {
        if (!err)
        {
            // Attempt a connection to each endpoint in the list until we
            // successfully establish a connection.
            /*
            asio::async_connect(socket_, endpoints,
                boost::bind(&client::handle_connect, this,
                    asio::placeholders::error));
                    */
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_connect(const asio::error_code& err)
    {
        if (!err)
        {
            // The connection was successful. Send the request.
            /*
            asio::async_write(socket_, request_,
                boost::bind(&client::handle_write_request, this,
                    asio::placeholders::error));
                    */
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_write_request(const asio::error_code& err)
    {
        if (!err)
        {
            // Read the response status line. The response_ streambuf will
            // automatically grow to accommodate the entire line. The growth may be
            // limited by passing a maximum size to the streambuf constructor.
            /*
            asio::async_read_until(socket_, response_, "\r\n",
                boost::bind(&client::handle_read_status_line, this,
                    asio::placeholders::error));
                    */
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_read_status_line(const asio::error_code& err)
    {
        if (!err)
        {
            // Check that response is OK.
            std::istream response_stream(&response_);
            std::string http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            std::string status_message;
            std::getline(response_stream, status_message);
            if (!response_stream || http_version.substr(0, 5) != "HTTP/")
            {
                std::cout << "Invalid response\n";
                return;
            }
            if (status_code != 200)
            {
                std::cout << "Response returned with status code ";
                std::cout << status_code << "\n";
                return;
            }

            // Read the response headers, which are terminated by a blank line.
            /*
            asio::async_read_until(socket_, response_, "\r\n\r\n",
                boost::bind(&client::handle_read_headers, this,
                    asio::placeholders::error));
                    */
        }
        else
        {
            std::cout << "Error: " << err << "\n";
        }
    }

    void handle_read_headers(const asio::error_code& err)
    {
        if (!err)
        {
            // Process the response headers.
            std::istream response_stream(&response_);
            std::string header;
            while (std::getline(response_stream, header) && header != "\r")
                std::cout << header << "\n";
            std::cout << "\n";

            // Write whatever content we already have to output.
            if (response_.size() > 0)
                std::cout << &response_;

            // Start reading remaining data until EOF.
            /*
            asio::async_read(socket_, response_,
                asio::transfer_at_least(1),
                boost::bind(&client::handle_read_content, this,
                    asio::placeholders::error));
                    */
        }
        else
        {
            std::cout << "Error: " << err << "\n";
        }
    }

    void handle_read_content(const asio::error_code& err)
    {
        if (!err)
        {
            // Write all of the data that has been read so far.
            std::cout << &response_;

            // Continue reading remaining data until EOF.
            /*
            asio::async_read(socket_, response_,
                asio::transfer_at_least(1),
                boost::bind(&client::handle_read_content, this,
                    asio::placeholders::error));
                    */
        }
        else if (err != asio::error::eof)
        {
            std::cout << "Error: " << err << "\n";
        }
    }

    tcp::resolver resolver_;
    tcp::socket socket_;
    asio::streambuf request_;
    asio::streambuf response_;
};

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

    struct HttpUrlConnection : public api::HttpUrlConnection {

        int32_t statusCode;
        std::string statusText;
        std::unordered_map<std::string, std::string> headers;
        std::vector<uint8_t> body;

        int32_t getStatusCode() override {
            return statusCode;
        }

        std::string getStatusText() override {
            return statusText;
        }

        std::unordered_map<std::string, std::string> getHeaders() override {
            return headers;
        }

        ledger::core::api::HttpReadBodyResult readBody() override {
            auto b = body;
            body = std::vector<uint8_t>();
            return api::HttpReadBodyResult(std::experimental::optional<ledger::core::api::Error>(), b);
        }
    };
}
