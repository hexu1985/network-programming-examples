#!/bin/bash

NET_ID="192.168.2"
NETMASK=24
START=10
END=26
IFNAME="eno1"

for (( i=$START; i<$END; i++ )); do
	IP="$NET_ID.$i"
	echo $IP
	ip addr add $IP/$NETMASK dev $IFNAME
    ./1000K_tcp_client "$IP" 192.168.2.2 8888 62500
done
