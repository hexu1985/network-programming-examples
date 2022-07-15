#!/usr/bin/env bash

H_NAME="unp.h"

ls -1 *.c | \
while read filename
do
    R_NAME=${filename%%.c}.h
    sed -i -e "s/\"$H_NAME\"/\"$R_NAME\"/g" $filename
done
