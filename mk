#!/bin/bash

full_name="$1"
params=$@
file_name=$(basename "$full_name")
file_name_without_extension="${file_name%.*}"
#libs=$(pkg-config --cflags --libs ncurses x11 wayland-client libevdev)
libs=$(pkg-config --cflags --libs libevdev ohmygame)
rm -f "$file_name_without_extension.tcc"
rm -f "$file_name_without_extension.gcc"
rm -f "$file_name_without_extension.clang"
rm -f "$file_name_without_extension"
flags="-Wall -fPIC"

rm -rf build
mkdir -p build

tcc $flags ohmygame/*.c $libs -o "build/$file_name_without_extension.tcc" $params
tcc $flags -static ohmygame/*.c $libs -o "build/$file_name_without_extension.static.tcc" $params
gcc $flags ohmygame/*.c $libs -o "build/$file_name_without_extension.gcc" $params
gcc $flags -static ohmygame/*.c $libs -o "build/$file_name_without_extension.static.gcc" $params
clang $flags ohmygame/*.c $libs -o "build/$file_name_without_extension.clang" $params
clang $flags -static ohmygame/*.c $libs -o "build/$file_name_without_extension.static.clang" $params
ls -la build/$file_name_without_extension*

