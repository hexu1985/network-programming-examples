#!/bin/bash
gcc mmap_write.c -o mmap_write 
gcc sendfile.c -o sendfile 
gcc client.c -o client
gcc server.c -o server
gcc splice_tcp_client.c -o splice_tcp_client
gcc splice_tcp_server.c -o splice_tcp_server
gcc splice_file.c -o splice_file
