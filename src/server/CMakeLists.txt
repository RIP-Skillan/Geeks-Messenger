add_executable(server_app 
    main.cpp 
    server.cpp
)
target_link_libraries(server_app PRIVATE chat_common ${Boost_LIBRARIES})
