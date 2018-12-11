#include <iostream>
#include <asio.hpp>
#include <memory>
#include "AsioExecutionContext.hpp"
#include "AsioHttpClient.hpp"

using namespace std;

int main()
{
    auto executionContext = std::make_shared<AsioExecutionContext>();

    executionContext->run();
    return 0;
}