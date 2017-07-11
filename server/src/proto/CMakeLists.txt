set(PROTO_FILES
    proto/core.proto
    proto/character.proto
    proto/user.proto)

# generate proto's
set(CMAKE_CURRENT_BINARY_DIR_SAFE ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR_SAFE})

add_library(proto STATIC
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ${PROTO_FILES})

message(${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(proto
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(proto protobuf::libprotobuf)
