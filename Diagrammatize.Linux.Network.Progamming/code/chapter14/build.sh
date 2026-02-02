#!/bin/bash
gcc xxxrlimit.c -o xxxrlimit 
gcc 1000K_socket.c -o 1000K_socket
gcc 1000K_tcp_client.c -o 1000K_tcp_client
gcc 1000K_tcp_server.c -o 1000K_tcp_server
gcc reactor_tcp_server.c -o reactor_tcp_server
gcc reactor_tcp_client.c -o reactor_tcp_client
