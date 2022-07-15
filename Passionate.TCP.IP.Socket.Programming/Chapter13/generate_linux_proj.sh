#!/usr/bin/env bash

current_dir=$(cd "$(dirname "$0")";pwd)
echo $current_dir

cmake -H"$current_dir/" -B"$current_dir/platforms_project/linux" -DCMAKE_BUILD_TYPE=Debug

