#!/usr/bin/env bash

X=20
Y=40
INIT=150

declare -A gol tmp

for ((; i++ < INIT;)) do
    ((
        x = RANDOM % X,
        y = RANDOM % Y,

        gol[$x,$y] = 1,
        tmp[$x,$y] = 1
    ))
done

check ()
{
    c=

    ((gol[$i,$j] == 1 && c--))

    for ((x = i - 1; x <= i + 1; x++)) do
        for ((y = j - 1; y <= j + 1; y++)) do
            ((
                x >= 0 && x < X &&
                y >= 0 && y < Y &&
                gol[$x,$y] == 1 && c++
            ))
        done
    done
}

update ()
{
    for ((i = 0; i < X; i++)) do
        for ((j = 0; j < Y; j++)) do
            check

            ((
                tmp[$i,$j] = c == 2 ?
                tmp[$i,$j] : c == 3 ? 1 : 0
            ))
        done
    done

    for i in "${!tmp[@]}"; do
        gol[$i]=${tmp[$i]}
    done
}

display ()
{
    p=

    for ((i = 0; i < X; i++)) do
        for ((j = 0; j < Y; j++)) do
            ((gol[$i,$j] == 1)) && p+=o || p+=' '
        done
        p+='\n'
    done

    printf '\e[0;0H%b' "$p"
}

printf '\e[2J'

for ((;;)) do
    display
    update
done
