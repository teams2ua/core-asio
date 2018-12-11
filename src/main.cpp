#include <iostream>
#include <asio.hpp>
#include <memory>
#include "AsioExecutionContext.hpp"
#include "AsioHttpClient.hpp"

using namespace std;

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

    asio::ip::tcp::resolver resolver_;
    asio::ip::tcp::socket socket_;
    asio::streambuf request_;
    asio::streambuf response_;
};


int main()
{
      
    
    return 0;
}