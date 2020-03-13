#!/usr/bin/env bash

# check for chicken
type csc &> /dev/null || {
    echo 'error : install chicken scheme first' >&2
    exit 1
}

printf -v src *.scm

bin=${src%.scm}

[[ -e $bin ]] && {
    {
        read -r a
        read -r b
    } < \
        <(stat -c '%Y' -- "$src" "$bin")


    ((a > b)) || exit
}

set -x

csc -O5 "$src" -o "$bin"
