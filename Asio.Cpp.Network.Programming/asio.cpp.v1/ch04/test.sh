#!/usr/bin/env bash                                                                                                             

########### test sync client and server ###############

./sync_server &
SRV_PID=$!

sleep 1

./sync_client 

sleep 1

kill $SRV_PID

sleep 1

########### test async client and server ###############

./async_server &
SRV_PID=$!

sleep 1

./async_client 

sleep 1

kill $SRV_PID

sleep 1

