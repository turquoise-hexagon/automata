#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

static const short DIRS[][2] = {
    {-1,  0},
    { 0, -1},
    { 1,  0},
    { 0,  1}
};

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 4)
        usage("[height] [width] [iter]", argv[0]);

    unsigned *args = argstous(argc, argv);

    /* init array */
    bool **uni = malloc(args[0] * sizeof *uni);

    if (uni == NULL)
        errx(EXIT_FAILURE, "program failed to allocate memory");

    for (unsigned i = 0; i < args[0]; ++i) {
        uni[i] = calloc(args[1], sizeof *uni[i]);

        if (uni == NULL)
            errx(EXIT_FAILURE, "program failed to allocate memory");
    }

    unsigned short dir = 0;
    unsigned x = args[0] / 2;
    unsigned y = args[1] / 2;

    /* run langton's ant */
    for (unsigned n = 0; n < args[2]; ++n) {
        printf("P1\n%u %u\n", args[1], args[0]);

        for (unsigned i = 0; i < args[0]; ++i)
            for (unsigned j = 0; j < args[1]; ++j)
                putchar(uni[i][j] + '0');

        dir = ((short)dir + ((uni[x][y] ^= 1) == 0 ? 1 : -1) + 4) % 4;

        x = ((long) x + DIRS[dir][0] + args[0]) % args[0];
        y = ((long) y + DIRS[dir][1] + args[1]) % args[1];
    }

    /* cleanup */
    for (unsigned i = 0; i < args[0]; ++i)
        free(uni[i]);

    free(args);
    free(uni);

    return EXIT_SUCCESS;
}
