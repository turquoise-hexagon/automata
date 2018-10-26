#!/usr/bin/env bash

X=20
Y=40
INIT=150

declare -A gol tmp

for ((; i++ < INIT;)) do
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
        ((gol[$x $((j - 1))] && c++))
        ((gol[$x $((j + 1))] && c++))
    done

    ((gol[$((i - 1)) $j] && c++))
    ((gol[$((i + 1)) $j] && c++))
}

update ()
{
    for ((i = 0; i < X; i++)) do
        for ((j = 0; j < Y; j++)) do
            check

            case $c in
                2) ;; # do nothing
                3) tmp[$i $j]=1;;
                *) tmp[$i $j]=0
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

    for ((i = 0; i < X; i++)) do
        for ((j = 0; j < Y; j++)) do
            ((gol[$i $j])) && p+=o || p+=' '
        done
        p+='\n'
    done

    printf '\e[H%b' "$p"
}

printf '\e[2J'

for ((;;)) do
    display
    update
done
