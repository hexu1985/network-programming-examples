# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(chapter-02 LANGUAGES C)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /utf-8")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /utf-8")
endif()

if(UNIX)
    set(LIBS_SYSTEM )
    set(SRC_FILE_LIST 
        oob_recv.c
        oob_recv_select.c
        oob_send.c
        oob_send_usleep.c
        peek_recv.c
        peek_send.c
        readv.c
        recv_select.c
        writev.c
        )
elseif(WIN32)
    set(LIBS_SYSTEM ws2_32)
    set(SRC_FILE_LIST
        oob_recv_win.c
        oob_send_win.c
        )
else()
    message(FATAL_ERROR "not support platform ${CMAKE_SYSTEM_NAME}")
endif()

foreach(SRC_FILE ${SRC_FILE_LIST})
    get_filename_component(TARGET_FILE_NAME ${SRC_FILE} NAME_WE)
    add_executable(${TARGET_FILE_NAME} ${SRC_FILE})
    target_link_libraries(${TARGET_FILE_NAME} ${LIBS_SYSTEM})
endforeach()

