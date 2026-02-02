#!/bin/bash
gcc unix_stream_server.c -o unix_stream_server
gcc unix_stream_client.c -o unix_stream_client
gcc unix_dgram_server.c -o unix_dgram_server
gcc unix_dgram_client.c -o unix_dgram_client
gcc socketpair_test.c -o socketpair_test
gcc pipe_test.c -o pipe_test
gcc trans_fds_server.c -o trans_fds_server
gcc trans_fds_client.c -o trans_fds_client
