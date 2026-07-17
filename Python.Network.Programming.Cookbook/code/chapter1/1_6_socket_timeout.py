#!/usr/bin/env python
# Python Network Programming Cookbook -- Chapter - 1
# This program is optimized for Python 2.7.
# It may run on any other version with/without modifications.

import socket

def test_socket_timeout():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Default socket timeout: {}".format(s.gettimeout()))
    s.settimeout(100)
    print("Current socket timeout: {}".format(s.gettimeout()))

if __name__ == '__main__':
    test_socket_timeout()
