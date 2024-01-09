#!/bin/bash

full_name="$1"
file_name=$(basename "$full_name")
file_name_without_extension="${file_name%.*}"
#libs=$(pkg-config --cflags --libs ncurses x11 wayland-client libevdev)
libs=$(pkg-config --cflags --libs libevdev)
tcc -Wall "$full_name" lib/*.c $libs -o "$file_name_without_extension"
echo $file_name_without_extension

