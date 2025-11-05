find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(proto_tests test_protocol.cpp)
target_link_libraries(proto_tests PRIVATE GTest::gtest_main chat_common)
gtest_discover_tests(proto_tests)

