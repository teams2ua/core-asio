cmake_minimum_required(VERSION 3.0)
project(syncbanch)

include_directories(../3rdparty/asio/include)
include_directories(../3rdparty/uri/include)
include_directories(../../../Ledger/lib-core/lib-core-build/include)

link_directories(../../../Ledger/lib-core/lib-core-build/core/src/build/)
link_directories(../3rdparty/uri/src/)

add_executable(syncbanch
main.cpp
AsioHttpClient.cpp
AsioExecutionContext.cpp
)
target_compile_definitions(syncbanch PUBLIC ASIO_STANDALONE)

target_link_libraries(syncbanch network-uri ledger-core)