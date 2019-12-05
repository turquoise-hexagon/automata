_() {
    declare -A array

    for src in src/*; {
        exec=${src##*/}
        exec=${exec%.c}

        if [[ -e $exec ]]; then
            {
                read -r a
                read -r b
            } < <(stat -c '%Y' -- "$src" "$exec")

            ((a > b)) && array[$src]=$exec
        else
            array[$src]=$exec
        fi
    }

    {
        set -x

        for src in "${!array[@]}"; {
            gcc $CFLAGS "$src" -o "${array[$src]}"
        }
    } |&
        while IFS= read -r line; do
            [[ $line =~ ^'+ '(for|cc) ]] ||
                printf '%s\n' "$line"
        done
}

# vim: ft=sh
