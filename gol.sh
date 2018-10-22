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

    for ((a = $1 - 1; a <= $1 + 1; a++)) do
        for ((b = $2 - 1; b <= $2 + 1; b++)) do
            ((
                a >= 0 && a < SIZE &&
                b >= 0 && b < SIZE
            )) &&
                ((${gol[$a,$b]} == 1)) && ((c++))
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
    for ((i = 0; i < SIZE; i++)) do
        for ((j = 0; j < SIZE; j++)) do
            ((${gol[$i,$j]} == 1)) && p+='o' || p+=' '
        done
        p+='\n'
    done

    printf '\e[0;0H%b' "$p"; p=
}

printf '\e[2J\e[H'

for ((;;)) { update; display; }
