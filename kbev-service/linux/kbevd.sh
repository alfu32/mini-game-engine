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

service_running=true
# Function to handle SIGINT (Ctrl+C)
interrupt_handler() {
  echo "Received Ctrl+C. Cleaning up and exiting..."
  # Add your cleanup code here if needed
  service_running=false
  exit 1
}

# Set a trap to call the interrupt_handler function when SIGINT is received
trap interrupt_handler SIGINT


current=$(pwd)
mkdir -p /tmp/kbev
cd /tmp/kbev
echo "configured events service in  : /tmp/kbev"

#ExecStartPre

input_devices=$(detect_input_devices)
echo $input_devices > input_devices.lst
echo "detected input devices : $input_devices"
echo "configuring service : "
chmod 777 input_devices.lst
touch events
chmod 777 events
rm -rf read.lock
ls -la

#ExecStart
while $service_running; do
    if [ -f read.lock ]
    then
      #nothing
      touch service.is.blocked
      sleep 0.01
    else
      rm -rf service.is.blocked
      touch read.lock

      for device in $(cat input_devices.lst); do
        cat $device > events
      done
      rm -rf read.lock
      sleep 0.04
      cat input_devices.lst
    fi
done

#ExecStop=

echo "keyboard event service exited"
cd $current