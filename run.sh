#!/bin/sh
g++ uhr.cpp -lSDL2 -o uhr
if (($? == 0)); then
    ./uhr
fi