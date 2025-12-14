cmake_minimum_required(VERSION 3.10)
project(server_app)

add_executable(server_app 
    main.cpp 
    server.cpp
    session.cpp
)
target_link_libraries(server_app PRIVATE chat_common ${Boost_LIBRARIES})

install(TARGETS server_app DESTINATION bin)
