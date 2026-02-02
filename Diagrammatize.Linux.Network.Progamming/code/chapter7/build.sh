#!/bin/bash
gcc udp_client.c -o udp_client
gcc udp_block_server.c -o udp_block_server
gcc udp_nonblock_server.c -o udp_nonblock_server
gcc udp_signal_server.c -o udp_signal_server
gcc io_uring_udp_server.c -o io_uring_udp_server -luring 
gcc io_uring_udp.c -o io_uring_udp -luring 
