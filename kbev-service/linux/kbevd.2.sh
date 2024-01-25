#!/bin/bash

# Function to detect input devices and return a list
get_keyboard_input_devices_list() {
  # initialize a list of input devices full paths
  input_devices_list=()
  # populate in
  for device in /dev/input/event*; do
    if udevadm info -a -n "$device" | grep -q "eyboard"; then
      input_devices_list+=("$device")
    fi
  done
  echo "${input_devices_list[@]}"
}

service_running=true
# Function to handle SIGINT (Ctrl+C)
interrupt_handler() {
  echo "Received Ctrl+C. Cleaning up and exiting..."
  # Add your cleanup code here if needed
  pkill cat
  exit 1
}

function main(){

  # Set a trap to call the interrupt_handler function when SIGINT is received
  trap interrupt_handler SIGINT

  #ExecStartPre

  input_devices=$(get_keyboard_input_devices_list)
  echo $input_devices > input_devices.lst
  echo "input devices $input "

  for device in $input_devices; do
    echo " - killing existing cat $device "
    pkill cat $device
    sleep 0.125
  done

  echo "removing exisiting pipe "
  rm -rf kbdpipe
  echo "creating output pipe kbdpipe"
  mkfifo kbdpipe
  echo "cconfiguring input polling"
  for device in $input_devices; do
    echo " - starting cat $device"
    cat $device > kbdpipe &
    sleep 0.125
  done
  echo "keyboard event service started"
}

main