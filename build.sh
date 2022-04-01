#!/bin/bash

mkdir -p build
cd build
cmake ..
make && printf '>>>>> Pass Created! <<<<<\n\n' || exit
cd ..
printf ">>>>> Compiling runtime library bb-counter-rt.c* <<<<<\n"
clang -c bb-counter-rt.c* && printf ">>>>> Success <<<<<\n\n" || exit
if [ "$#" -lt 1 ]; then
    echo "You must pass target program file name: ./build.sh filename"
else
    echo ">>>>> Instrumenting input source files <<<<<"
    for i in "$@"    # same as your "for i"
    do
        # echo "$i"
        clang -fno-discard-value-names -Xclang -load -Xclang build/counter-pass/libBBCounterPass.so -c $i
    done
    echo ">>>>> Linking all the objects <<<<<"
    clang *.o -o bb_count_instrumented.out
fi

echo ">>>>>Done!<<<<<"
