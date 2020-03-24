#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 3)
        usage("[rule] [init]", argv[0]);

    errno = 0;
    long tmp;
    char* ptr;

    tmp = strtol(argv[1], &ptr, 10);

    if (errno != 0 || *ptr != 0 || tmp < 0 || tmp > 255)
        errx(EXIT_FAILURE, "'%s' isn't a valid rule", argv[1]);

    const unsigned rule = (unsigned)tmp;

    const size_t length = strnlen(argv[2], LINE_MAX);

    /* init array */
    bool **uni = malloc(length * sizeof *uni);

    if (uni == NULL)
        errx(EXIT_FAILURE, "program failed to allocate memory");

    bool flag = 0;

    for (size_t i = 0; i < length; ++i) {
        if ((uni[i] = malloc(2 * sizeof *uni[i])) == NULL)
            errx(EXIT_FAILURE, "program failed to allocate memory");

        switch (argv[2][i]) {
            case '0' : uni[i][flag] = 0; break;
            case '1' : uni[i][flag] = 1; break;
            default  : errx(EXIT_FAILURE, "'%c' invalid value found in strip", argv[2][i]);
        }
    }

    /* run cellular automaton */
    printf("P1\n%lu %lu\n", length, length);

    unsigned short cnt;

    for (size_t i = 0; i < length; ++i) {
        for (size_t j = 0; j < length; ++j) {
            putchar(uni[j][flag] + '0');

            cnt = 0;

            for (short k = -1; k < 2; ++k)
                cnt = cnt << 1 | uni[((long)j + k + length) % length][flag];

            uni[j][!flag] = 1 & rule >> cnt;
        }

        flag ^= 1;
    }

    /* cleanup */
    for (size_t i = 0; i < length; ++i)
        free(uni[i]);

    free(uni);

    return EXIT_SUCCESS;
}
