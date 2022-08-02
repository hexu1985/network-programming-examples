#!/usr/bin/env bash

./SockShutdownServer &
sleep 1
./SockShutdownClient

