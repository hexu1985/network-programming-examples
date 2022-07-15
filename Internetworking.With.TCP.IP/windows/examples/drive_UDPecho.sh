#!/usr/bin/env bash

SRV_PORT=$(($RANDOM + 1024))

./UDPechod $SRV_PORT &
SRV_PID=$!

sleep 1

./UDPecho 127.0.0.1 $SRV_PORT <<EOF
hello
world
bye
EOF

kill $SRV_PID
