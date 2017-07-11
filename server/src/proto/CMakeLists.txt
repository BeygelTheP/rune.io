set(PROTO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(PROTO_FILES
    proto/core.proto
    proto/character.proto
    proto/user.proto)

# generate proto's
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(proto STATIC ${PROTO_SRCS})

message(${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(proto
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROTO_INCLUDE_DIR})

target_link_libraries(proto protobuf::libprotobuf)
