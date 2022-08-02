#!/usr/bin/env bash

########### test async wait signal ################

./async_wait_signal &
SRV_PID=$!

sleep 1

kill -SIGINT $SRV_PID

########### test io_service_run ################

./tcp_async_accept &
SRV_PID=$!

sleep 1

./io_service_run1

sleep 1

./io_service_run2

sleep 1

./io_service_run3

sleep 1

kill -SIGINT $SRV_PID

