#!/usr/bin/bash


for device in /dev/input/event*; do
    name=$(udevadm info -a -n "$device" | grep "eyboard")
    echo "$device       $name"
done