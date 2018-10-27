#!/usr/bin/env bash

X=20
Y=40
I=150

declare -A gol cpy

for ((; i++ < I;)) do
    ((
        x = RANDOM % X,
        y = RANDOM % Y,

        gol[$x $y] = 1
    ))
done

check ()
{
    c=

    for ((x = i - 1; x <= i + 1; x++)) do
        ((
            gol[$x $((j - 1))] && c++,
            gol[$x $((j + 1))] && c++
        ))
    done

    ((
        gol[$((i - 1)) $j] && c++,
        gol[$((i + 1)) $j] && c++
    ))
}

update ()
{
    p=

    for ((i = 0; i < X; i++)) do
        for ((j = 0; j < Y; j++)) do
            check

            case $c in
                2) ;; # do nothing
                3) cpy[$i $j]=1;;
                *) cpy[$i $j]=0
            esac

            ((cpy[$i $j])) && p+=o || p+=' '
        done

        p+='\n'
    done

    printf '\e[H%b' "$p"

    for i in "${!cpy[@]}"; do
        gol[$i]=${cpy[$i]}
    done
}

printf '\e[2J'

for ((;;)) do
    update
done
