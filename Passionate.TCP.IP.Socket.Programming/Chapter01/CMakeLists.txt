# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(chapter-01 LANGUAGES C)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /utf-8")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /utf-8")
endif()

if(UNIX)
    add_executable(fd_seri fd_seri.c)
    add_executable(hello_client hello_client.c)
    add_executable(hello_server hello_server.c)
    add_executable(low_open low_open.c)
    add_executable(low_read low_read.c)
elseif(WIN32)
    add_executable(hello_client_win hello_client_win.c)
    target_link_libraries(hello_client_win ws2_32)

    add_executable(hello_server_win hello_client_win.c)
    target_link_libraries(hello_server_win ws2_32)
else()
    message(FATAL_ERROR "not support platform ${CMAKE_SYSTEM_NAME}")
endif()

