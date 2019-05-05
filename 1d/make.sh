#!/bin/sh
#
# make script

set -xe

cc() {
    gcc -pedantic -Wall -Wextra \
        -march=native -O3 "$@"
}

cd src

cc -c ./*.c
cc -o ../automaton ./*.o

rm ./*.o
