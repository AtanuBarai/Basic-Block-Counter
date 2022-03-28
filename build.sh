#!/bin/bash

mkdir -p build
cd build
cmake ..
make
cd ..
clang -c bb-counter-rt.c
if [ "$#" -lt 1 ]; then
    echo "You must pass target program file name: ./build.sh filename"
else
    for i in "$@"    # same as your "for i"
    do
        # echo "$i"
        clang -fno-discard-value-names -Xclang -load -Xclang build/counter-pass/libBBCounterPass.so -c $i
    done

    clang *.o -o bb_count_instrumented.out
fi
