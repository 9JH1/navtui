#!/bin/bash 
gcc -o src/navtui *.c json/cjson.c $(pkg-config --cflags --libs libgcrypt libcurl)
./src/navtui
