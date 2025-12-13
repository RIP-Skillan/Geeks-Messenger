cmake_minimum_required(VERSION 3.10)
project(client_app)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(client_app main.cpp client.cpp)

# Link directly with the found libraries instead of Boost::system
target_include_directories(client_app PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(client_app PRIVATE ${Boost_LIBRARIES} pthread)
