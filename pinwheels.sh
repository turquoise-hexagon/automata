#!/usr/bin/env bash
#
# @laserbat's cellular automaton

X=30
Y=30
I=300

# rules
rule_m='--+---00000+---++'
rule_d='------00000++++++'
rule_p='--+++-00000+++-++'

declare -A uni cpy

for((; i < I; i++)) {
    ((
        x = RANDOM % X,
        y = RANDOM % Y,

        uni[$x $y] = (RANDOM % 2) + 1
    ))
}

printf '\e[2J\e[?25l'
trap $'printf \e[?25h' EXIT

for ((;;)) {
    out='\e[H'

    for ((i = 0; i < X; i++)) {
        for ((j = 0; j < Y; j++)) {
            case ${uni[$i $j]} in
                1) out+='\e[31m- \e[m';;
                2) out+='\e[32m+ \e[m';;
                *) out+='\e[30m  \e[m'
            esac

            cpt=0

            for a in -1 0 1; {
                for b in -1 0 1; {
                    ((a || b)) && {
                        ((
                            x = i + a, x = x < 0 ? X - 1 : x % X,
                            y = j + b, y = y < 0 ? Y - 1 : y % Y
                        ))

                        case ${uni[$x $y]} in
                            1) ((cpt--));;
                            2) ((cpt++))
                        esac
                    }
                }
            }

            case ${uni[$i $j]} in
                1) : ${rule_m:cpt + 8:1};;
                2) : ${rule_p:cpt + 8:1};;
                *) : ${rule_d:cpt + 8:1}
            esac

            case $_ in
                -) cpy[$i $j]=1;;
                +) cpy[$i $j]=2;;
                0) cpy[$i $j]=0
            esac
        }

        out+='\n'
    }

    printf '%b' "$out"

    for i in "${!cpy[@]}"; {
        uni[$i]=${cpy[$i]}
    }
}