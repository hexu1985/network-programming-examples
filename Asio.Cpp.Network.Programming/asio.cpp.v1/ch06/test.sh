#!/usr/bin/env bash

########### test tcp sync echo ################

../ch03/tcp_sync_echo_server &
SRV_PID=$!

sleep 1

./tcp_sync_echo_client 

sleep 1

./tcp_sync_echo_client2

kill $SRV_PID

sleep 1

########### test async client and server ###############

../ch04/async_server &
SRV_PID=$!

sleep 1

./coroutines

sleep 1

kill $SRV_PID

sleep 1

