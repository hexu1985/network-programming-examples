#!/usr/bin/env bash                                                                                                             

########### test tcp sync echo ################

./tcp_sync_echo_server &
SRV_PID=$!

sleep 1

./tcp_sync_echo_client 

sleep 1

kill $SRV_PID

sleep 1

########### test tcp async echo ################

./tcp_async_echo_server &
SRV_PID=$!

sleep 1

./tcp_async_echo_client 

sleep 1

kill $SRV_PID

########### test udp sync echo ################

./udp_sync_echo_server &
SRV_PID=$!

sleep 1

./udp_sync_echo_client 

sleep 1

kill $SRV_PID

sleep 1

