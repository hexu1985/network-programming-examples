#!/bin/bash

TIMES=30
PROCESS="udp_client"

stop_test() {
    for (( i=0; i<$TIMES; i++ )); do
        pid=$(ps -aux | grep $PROCESS | grep -v grep | awk '{print $2}' | head -n 1)
        if [ -n "$pid" ]; then
            kill -9 $pid
            echo "进程:$pid 已被杀死"
        else
            echo "未找到进程"
        fi
        sleep 0.1
    done
}

start_test() {
    for (( i=0; i<$TIMES; i++ )); do
        ./$PROCESS $PEER_IP $PEER_PORT &
        sleep 0.1
    done
}

usage() {
    echo "./net_test.sh start 服务端IP 服务端端口"
    echo "./net_test.sh stop"
}

if [[ $1 != "start" && $1 != "stop" ]]; then
    usage
    exit
fi

ACTION=$1
PEER_IP=$2
PEER_PORT=$3

if [ $ACTION = "start" ]; then
    start_test $PEER_IP $PEER_PORT
else
    stop_test
fi
