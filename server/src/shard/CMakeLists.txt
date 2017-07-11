set(SOURCES
    main.cpp
    channel/channel.cpp
    server/connection.cpp
    server/conn_group.cpp
    server/conn_manager.cpp
    server/ws_server.cpp
    world/character.cpp
    world/world.cpp)

set(HEADERS
    channel/channel.hpp
    channel/conn_group_message.hpp
    channel/message.hpp
    server/connection.hpp
    server/conn_manager.hpp
    world/character.hpp
    channel/connection_message.hpp
    channel/conn_state_message.hpp
    channel/network_message.hpp
    server/conn_group.hpp
    server/ws_server.hpp
    world/world.hpp)

add_executable(shard ${SOURCES} ${HEADERS})

target_include_directories(proto
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(shard
    proto
    websocketpp::websocketpp
    protobuf::libprotobuf
    Boost::boost
    Boost::system
    Boost::thread)
