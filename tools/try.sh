# !/usr/bin/bash
if [[ -z "$1" ]]; then
    name="test"
else
    name="$1"
fi
clear
g++ ./try/try.cpp -o ./try/try.out
./try/try.out
rm ./try/try.out