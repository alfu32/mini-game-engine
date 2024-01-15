#!/bin/bash

# Function to detect input devices and return a list
detect_input_devices() {
  input_devices=()
  for device in /dev/input/event*; do
    if udevadm info -a -n "$device" | grep -q "eyboard"; then
      input_devices+=("$device")
    fi
  done
  echo "${input_devices[@]}"
}

for device in $(detect_input_devices); do
  echo $device
done