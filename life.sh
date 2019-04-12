#!/usr/bin/env bash
#
# game of life

X=20
Y=20
I=150

declare -A gol cpy

for ((; i++ < I;)) {
    ((
        x = RANDOM % X,
        y = RANDOM % Y,

        gol[$x $y] = 1
    ))
}

printf '\e[2J\e[?25l'
trap $'printf \e[?25h' EXIT

for ((;;)) {
    out='\e[H'

    for ((i = 0; i < X; i++)) {
        for ((j = 0; j < Y; j++)) {
            ((gol[$i $j])) && out+='# ' || out+='  '

            cpt=0

            for a in -1 0 1; {
                for b in -1 0 1; {
                    ((a || b)) && {
                        ((
                            x = i + a, x = x < 0 ? X - 1 : x % X,
                            y = j + b, y = y < 0 ? Y - 1 : y % Y
                        ))

                        ((gol[$x $y] && cpt++))
                    }
                }
            }

            case $cpt in
                2) ;; # do nothing
                3) cpy[$i $j]=1;;
                *) cpy[$i $j]=0
            esac
        }

        out+='\n'
    }

    printf '%b' "$out"

    for i in "${!cpy[@]}"; do
        gol[$i]=${cpy[$i]}
    done
}
