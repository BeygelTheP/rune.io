cmake_minimum_required(VERSION 3.9)

project(Server
  VERSION 0.0.3
  LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin/${CMAKE_BUILD_TYPE}/")

# setup boost
find_package(Boost 1.62.0
    COMPONENTS thread system REQUIRED)

# setup websocketpp (h-only lib)
find_package(WebsocketPP REQUIRED)

# setup protobuf
find_package(Protobuf 3.3.0 REQUIRED)

# setup proto
add_subdirectory(proto)

# setup modules
add_subdirectory(shard)
