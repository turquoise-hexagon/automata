#!/usr/bin/env bash
#
# generate xbm files

set -e

if (($# != 3)); then
    printf 'usage : gen.sh [rule] [width] [height]'
    exit 1
fi

if [[ ! -x automaton ]]; then
    ./make.sh &> /dev/null
fi

{
    mapfile -t input
     
    printf '#define _width %04s\n' "$2"
    printf '#define _height%04s\n' "$3"
    printf '\nstatic unsigned char _bits[] = {\n'
    
    for ((; i < $3; i++)) {
        for ((j = 0; j < $2; j += 8)) {
            tmp=${input[i]:j:8}
            rev=

            for ((k = ${#tmp} - 1; k >= 0; k--)) {
                rev+=${tmp:k:1}
            }

            printf -v tmp '0x%02x' $((2#$rev))

            ((i + 1 < $3 || j + 8 < $2)) &&
                printf '%s, '   "$tmp" ||
                printf '%s\n};' "$tmp"
        }
    }

    printf '%b' "$out"
} < <(
    while read -r line; do
        printf '%s\n' "${line:${#line} - $2 - 2:$2}"
    done < <(
        {
            mapfile -t input

            printf '%s\n' "${input[@]:${#input[@]} - $3:$3}"
        } < <(
            ./automaton "$1" $(
                for ((; i++ < $2 * 2;)) {
                    printf '%s' $((RANDOM % 2))
                }
            )))) > "etc/$1.xbm"
