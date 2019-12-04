#!/usr/bin/env bash

set -e

cd src

for file in *.c; {
    exec=../${file%.c}

    if [[ -e $exec ]]; then
        {
            read -r a
            read -r b
        } < <(stat -c '%Y' -- "$file" "$exec")

        ((a > b)) && array+=("$file")
    else
        array+=("$file")
    fi
}

{
    set -x

    for file in "${array[@]}"; {
        gcc $CFLAGS "$file" -o "../${file%.c}"
    }
} |&
    while IFS= read -r line; do
        [[ $line =~ ^'+ '(for|cc) ]] ||
            printf '%s\n' "$line"
    done
