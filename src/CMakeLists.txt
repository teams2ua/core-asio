cmake_minimum_required(VERSION 3.0)
project(asio_http)

include_directories(../3rdparty/asio/include)
include_directories(../3rdparty/uri/include)
include_directories(${LEDGER_CORE_BLD_PATH}/include/ledger/core/)
include_directories(${LEDGER_CORE_BLD_PATH}/include/ledger/core/export/)


link_directories(${LEDGER_CORE_BLD_PATH}/core/src/)
link_directories(../3rdparty/uri/src/)

add_library(asio_http
AsioHttpClient.cpp
AsioExecutionContext.cpp
RequestResponce.cpp
)
target_compile_definitions(asio_http PUBLIC ASIO_STANDALONE)

target_link_libraries(asio_http network-uri ledger-core)
