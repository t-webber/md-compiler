#!/usr/bin/bash
clear
xmake
if [[ -z $1 ]]
then
name="test/file.md"
else
name=$1
fi
./build/linux/x86_64/release/mdCompiler $name