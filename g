#!/bin/bash
clear
name=$1
if [[ $name -eq '' ]]
then
name="md"
fi
g++ -Wall -pedantic -g -O0 "$name.cpp" -o "out"
./out file.md
