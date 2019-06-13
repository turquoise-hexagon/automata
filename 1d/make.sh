#!/bin/sh
#
# make script

set -xe

cc() {
    gcc               \
        -pedantic     \
        -Wall         \
        -Wextra       \
        -static       \
        -march=native \
        -Ofast "$@"
}

cd src

cc -c ./*.c
cc -o ../automaton ./*.o

rm ./*.o
