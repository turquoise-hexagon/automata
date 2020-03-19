#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 3) {
        fprintf(stderr, "usage: %s [rule] [strip]\n", basename(argv[0]));

        return 1;
    }

    size_t i, j;

    errno = 0;
    char *ptr;

    const long rule = strtol(argv[1], &ptr, 10);

    if (errno != 0 || *ptr != 0 || rule < 0 || rule > 255)
        errx(1, "'%s' isn't a valid rule", argv[1]);

    const size_t length = strlen(argv[2]);

    /* init arrays */
    bool **strip = malloc(length * sizeof *strip);

    if (strip == NULL)
        errx(1, "program failed to allocate memory");

    for (i = 0; i < length; ++i) {
        strip[i] = malloc(2 * sizeof *strip[i]);

        if (strip[i] == NULL)
            errx(1, "program failed to allocate memory");
    }

    bool flag = false;

    for (i = 0; i < length; ++i)
        switch (argv[2][i]) {
            case '0' : strip[i][flag] = false; break;
            case '1' : strip[i][flag] = true;  break;
            default  :
                for (i = 0; i < length; ++i)
                    free(strip[i]);

                free(strip);

                errx(1, "'%s' isn't a valid strip", argv[2]);
        }

    printf("P1\n%lu %lu\n", length, length);

    /* run cellular automaton */
    unsigned short tmp;

    for (i = 0; i < length; ++i) {
        for (j = 0; j < length; ++j) {
            tmp = 0;

            putchar(strip[j][flag] == true ? '1' : '0');

            for (short k = -1; k <= 1; ++k)
                tmp = tmp << 1 | strip[(j + k + length) % length][flag];

            strip[j][!flag] = 1 & rule >> tmp;
        }

        flag ^= true;
    }

    /* cleanup */
    for (i = 0; i < length; ++i)
        free(strip[i]);

    free(strip);

    return 0;
}
