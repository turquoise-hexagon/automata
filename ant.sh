#!/usr/bin/env bash
#
# langton's ant

X=40
Y=40

display() {
    out='\e[H'

    for ((i = 0; i < X; i++)) {
        for ((j = 0; j < Y; j++)) {
            if ((i == ant[0] && j == ant[1])); then
                out+='\e[31m# \e[m'
            else
                ((world[$i $j])) && out+='. ' || out+='  '
            fi
        }

        out+='\n'
    }

    printf '%b' "$out"
}

update() {
    ((
        ant[2] = world[${ant[0]} ${ant[1]}] ? (ant[2] + 1) % 4 : (ant[2] < 1 ? 4 : ant[2]) - 1,
        world[${ant[0]} ${ant[1]}] ^=1
    ))

    case ${ant[2]} in
        0) ((ant[0]--));;
        1) ((ant[1]++));;
        2) ((ant[0]++));;
        3) ((ant[1]--))
    esac

    ((
        ant[0] = ant[0] < 0 ? X - 1 : ant[0] % X,
        ant[1] = ant[1] < 0 ? Y - 1 : ant[1] % Y
    ))
}

declare -A world

((
    ant[0] = RANDOM % X,
    ant[1] = RANDOM % Y,
    ant[2] = RANDOM % 4
))

echo -e '\e[2J\e[?25l'
trap $'echo -e \e[?25h' EXIT

for ((;;)) {
    display
    update
}
