#!/usr/bin/env bash

current_dir=$(cd "$(dirname "$0")";pwd)
echo $current_dir

cmake -H"$current_dir/" -B"$current_dir/platforms_project/win32" -DCMAKE_CONFIGURATION_TYPES="Release;Debug"


