#!/usr/bin/env bash
#
# a game of life

SIZE=30
INIT=100

declare -A gol tmp

for ((i = 0; i < SIZE; i++)) do
    for ((j = 0; j < SIZE; j++)) do
        gol[$i,$j]=0
        tmp[$i,$j]=0
    done
done

for ((i = 0; i < INIT; i++)) do
    a=$((RANDOM % SIZE))
    b=$((RANDOM % SIZE))

    gol[$a,$b]=1
    tmp[$a,$b]=1
done

check ()
{
    c=0

    ((${gol[$1,$2]} == 1)) && ((c--))

    for ((x = $1 - 1; x <= $1 + 1; x++)) do
        for ((y = $2 - 1; y <= $2 + 1; y++)) do
            ((
                x >= 0 && x < SIZE &&
                y >= 0 && y < SIZE
            )) &&
                ((${gol[$x,$y]} == 1)) && ((c++))
        done
    done
}

update ()
{
    for ((i = 0; i < SIZE; i++)) do
        for ((j = 0; j < SIZE; j++)) do
            check "$i" "$j"

            case $c in
                2) ;; # do nothing
                3) tmp[$i,$j]=1;;
                *) tmp[$i,$j]=0
            esac
        done
    done

    for i in "${!tmp[@]}"; { gol[$i]=${tmp[$i]}; }
}

display ()
{
    p=

    for ((i = 0; i < SIZE; i++)) do
        for ((j = 0; j < SIZE; j++)) do
            ((${gol[$i,$j]} == 1)) && p+='o' || p+=' '
        done
        p+='\n'
    done

    printf '\e[0;0H%b' "$p"
}

printf '\e[2J\e[H'

while :; do
    update
    display
done
