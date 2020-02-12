CFLAGS+=' -pedantic -Wall -Wextra -static -Ofast -march=native'

_() {
    declare -A array

    # build list of files to compile
    for src in src/*; {
        bin=${src##*/}
        bin=${bin%.c}

        [[ -e $bin ]] && {
            {
                read -r a
                read -r b
            } < \
                <(stat -c '%Y' -- "$src" "$bin")

            ((a > b)) || continue
        }

        array[$src]=$bin
    }

    {
        set -x

        # compile
        for src in "${!array[@]}"; {
            gcc $CFLAGS "$src" -o "${array[$src]}"
        }
    } |&
        # pretty printing
        while IFS= read -r line; do
            [[ $line =~ ^'+ '(for|cc) ]] ||
                printf '%s\n' "$line"
        done
}

# vim: ft=sh
