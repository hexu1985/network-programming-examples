#!/usr/bin/env bash
./async_op

########### test async client and server ###############

./async_server_multi_threaded &
SRV_PID=$!

sleep 1

../ch04/async_client 

sleep 1

kill $SRV_PID

sleep 1

