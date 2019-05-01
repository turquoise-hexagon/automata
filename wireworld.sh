#!/usr/bin/env bash
#
# wireworld

mapfile -t input

# sample config file :
# 00000000000000000000000000000000000000000
# 00000000000000000000000000000000000000000
# 00000000000321333320000000000000000000000
# 00000000003000000001333330000000000000000
# 00000000000333333330000003000000000000000
# 00000000000000000000000033330000000000000
# 00000000000000000000000030033333000000000
# 00000000000000000000000033330000000000000
# 00000000000321333330000001000000000000000
# 00000000003000000003333320000000000000000
# 00000000000331233330000000000000000000000
# 00000000000000000000000000000000000000000
# 00000000000000000000000000000000000000000

declare -A uni cpy

X=${#input[@]}
Y=${#input[0]}

for ((i = 0; i < X; i++)) {
    for ((j = 0; j < Y; j++)) {
        uni[$i $j]=${input[i]:j:1}
    }
}

enable -f /usr/lib/bash/sleep sleep

printf '\e[2J\e[?25l'
trap $'printf \e[?25h' EXIT

for ((;;)) {
    out='\e[H'

    for ((i = 0; i < X; i++)) {
        for ((j = 0; j < Y; j++)) {
            cpt=0

            case ${uni[$i $j]} in
                1) out+='\e[44m \e[m'; cpy[$i $j]=2;;
                2) out+='\e[41m \e[m'; cpy[$i $j]=3;;
                3) out+='\e[43m \e[m'

                    for a in -1 0 1; {
                        for b in -1 0 1; {
                            ((a || b)) && {
                                ((
                                    x = i + a, x = x < 0 ? X - 1 : x % X,
                                    y = j + b, y = y < 0 ? Y - 1 : y % Y
                                ))

                                ((uni[$x $y] == 1 && cpt++))
                            }
                        }
                    }

                    ((cpt && cpt < 3)) && cpy[$i $j]=1
                    ;;
                0) out+='\e[40m \e[m'
            esac
        }

        out+='\n'
    }

    printf '%b' "$out"

    for i in "${!cpy[@]}"; {
        uni[$i]=${cpy[$i]}
    }

    sleep .2
}