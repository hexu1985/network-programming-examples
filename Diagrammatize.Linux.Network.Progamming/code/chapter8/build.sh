#!/bin/bash
gcc tcp_client.c -o tcp_client
gcc tcp_select.c -o tcp_select
gcc udp_client.c -o udp_client
gcc udp_select.c -o udp_select
