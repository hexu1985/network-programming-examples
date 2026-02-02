#!/bin/bash
gcc udp_client.c -o udp_client
gcc tcp_client.c -o tcp_client
gcc tcp_iterative_client.c -o tcp_iterative_client
gcc udp_iterative_server.c -o udp_iterative_server
gcc tcp_iterative_server.c -o tcp_iterative_server
gcc udp_process_server.c -o udp_process_server
gcc tcp_process_server.c -o tcp_process_server
gcc udp_thread_server.c -o udp_thread_server
gcc tcp_thread_server.c -o tcp_thread_server
