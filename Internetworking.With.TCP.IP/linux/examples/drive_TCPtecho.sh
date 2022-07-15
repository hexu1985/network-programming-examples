#!/usr/bin/env bash

./TCPmechod &
SRV_PID=$!

sleep 1

./TCPtecho 127.0.0.1 127.0.0.2 127.0.0.3

kill $SRV_PID
