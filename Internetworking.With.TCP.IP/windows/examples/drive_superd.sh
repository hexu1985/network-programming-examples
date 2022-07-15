#!/usr/bin/env bash

./superd &
SRV_PID=$!

sleep 1

./TCPecho <<EOF
hello
world
bye
EOF

./TCPdaytime 127.0.0.1
./TCPtime 127.0.0.1

kill $SRV_PID
