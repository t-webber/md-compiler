#!/usr/bin/bash
clear
xmake

if [[ -z $1 ]]
then
    name="test/file.md"
else
    name=$1
fi

case $name in
    "test/file.md")
        echo "Processing test/file.md..."
        ;;
    "another/file.md")
        echo "Processing another/file.md..."
        ;;
    *)
        echo "Unknown file: $name"
        ;;
esac

./build/linux/x86_64/release/mdCompiler $name


