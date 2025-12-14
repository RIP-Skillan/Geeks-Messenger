# Finding protobuf
find_package(Protobuf REQUIRED)

# Run protoc on .proto file(s)
protobuf_generate_cpp(GENERATED_SRCS GENERATED_HDRS protocol.proto)

# Creating a static library containing the generated code
add_library(chat_common STATIC ${GENERATED_SRCS} ${GENERATED_HDRS})

# Making the generated headers public
target_include_directories(chat_common PUBLIC ${Protobuf_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

# Link against protobuf library itself
target_link_libraries(chat_common PUBLIC ${Protobuf_LIBRARIES})


