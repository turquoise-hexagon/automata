_() {
    declare -A array

    for src in src/*; {
        bin=${src##*/}
        bin=${bin%.c}

        if [[ -e $bin ]]; then
            {
                read -r a
                read -r b
            } < <(stat -c '%Y' -- "$src" "$bin")

            ((a > b)) && array[$src]=$bin
        else
            array[$src]=$bin
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
