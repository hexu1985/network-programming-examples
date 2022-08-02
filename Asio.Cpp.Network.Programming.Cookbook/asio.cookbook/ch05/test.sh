#!/usr/bin/env bash

########### test http client and server ###############

./HTTPServer &
SRV_PID=$!

sleep 1

./HTTPClient

sleep 1

kill $SRV_PID

sleep 1

