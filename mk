#!/bin/bash

full_name="$1"
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

tcc $flags "$full_name" ohmygame/*.c $libs -o "build/$file_name_without_extension.tcc"
tcc $flags -static "$full_name" ohmygame/*.c $libs -o "build/$file_name_without_extension.static.tcc"
gcc $flags "$full_name" ohmygame/*.c $libs -o "build/$file_name_without_extension.gcc"
gcc $flags -static "$full_name" ohmygame/*.c $libs -o "build/$file_name_without_extension.static.gcc"
clang $flags "$full_name" ohmygame/*.c $libs -o "build/$file_name_without_extension.clang"
clang $flags -static "$full_name" ohmygame/*.c $libs -o "build/$file_name_without_extension.static.clang"
ls -la build/$file_name_without_extension*

