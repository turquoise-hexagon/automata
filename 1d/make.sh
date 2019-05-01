#!/bin/sh
#
# make script

set -xe

cc() {
    gcc -pedantic -Wall -Wextra "$@"
}

cd src

cc -c ./*.c
cc -o ../automaton ./*.o

rm ./*.o
