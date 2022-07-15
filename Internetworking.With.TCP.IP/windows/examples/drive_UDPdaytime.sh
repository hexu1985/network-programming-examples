#!/usr/bin/env bash

SRV_PORT=$(($RANDOM + 1024))
./UDPdaytimed $SRV_PORT &
SRV_PID=$!
sleep 1
./UDPdaytime 127.0.0.1 $SRV_PORT
kill $SRV_PID
