#!/usr/bin/env bash

SIZE=30
INIT=100

declare -A gol tmp

for ((; i++ < INIT;)) do
    ((
        x = RANDOM % SIZE,
        y = RANDOM % SIZE,

        gol[$x,$y] = 1,
        tmp[$x,$y] = 1
    ))
done

check ()
{
    c=

    ((gol[$1,$2] == 1 && c--))

    for ((x = $1 - 1; x <= $1 + 1; x++)) do
        for ((y = $2 - 1; y <= $2 + 1; y++)) do
            ((
                x >= 0 && x < SIZE &&
                y >= 0 && y < SIZE &&
                gol[$x,$y] == 1 && c++
            ))
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

    for i in "${!tmp[@]}"; do
        gol[$i]=${tmp[$i]}
    done
}

display ()
{
    p=

    for ((i = 0; i < SIZE; i++)) do
        for ((j = 0; j < SIZE; j++)) do
            ((gol[$i,$j] == 1)) && p+=o || p+=' '
        done
        p+='\n'
    done

    printf '\e[0;0H%b' "$p"
}

printf '\e[2J'

for ((;;)) do
    update
    display
done
