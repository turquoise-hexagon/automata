#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 5)
        usage("[height] [width] [init] [iter]", argv[0]);

    unsigned *args = argstous(argc, argv);

    /* init array */
    bool ***uni = malloc(args[0] * sizeof *uni);

    if (uni == NULL)
        errx(EXIT_FAILURE, "program failed to allocate memory");

    for (unsigned i = 0; i < args[0]; ++i) {
        uni[i] = malloc(args[1] * sizeof *uni[i]);

        if (uni[i] == NULL)
            errx(EXIT_FAILURE, "program failed to allocate memory");

        for (unsigned j = 0; j < args[1]; ++j) {
            uni[i][j] = calloc(2, sizeof *uni[i][j]);

            if (uni[i][j] == NULL)
                errx(EXIT_FAILURE, "program failed to allocate memory");
        }
    }

    bool flag = 0;
    unsigned x, y;
    srand(time(NULL));

    for (unsigned i = 0; i < args[2]; ++i) {
        x = rand() % args[0];
        y = rand() % args[1];

        uni[x][y][flag] = 1;
    }

    /* run the game of life */
    bool tmp;
    unsigned short cnt;

    for (unsigned n = 0; n < args[3]; ++n) {
        printf("P1\n%u %u\n", args[1], args[0]);

        for (unsigned i = 0; i < args[0]; ++i)
            for (unsigned j = 0; j < args[1]; ++j) {
                putchar(uni[i][j][flag] + '0');

                cnt = 0;

                for (short u = -1; u < 2; ++u)
                    for (short v = -1; v < 2; ++v)
                        if (u != 0 || v != 0) {
                            x = ((long)i + u + args[0]) % args[0];
                            y = ((long)j + v + args[1]) % args[1];

                            cnt += uni[x][y][flag];
                        }

                switch (cnt) {
                    case 2  : tmp = uni[i][j][flag]; break;
                    case 3  : tmp = 1;               break;
                    default : tmp = 0;
                }

                uni[i][j][!flag] = tmp;
            }

        flag ^= 1;
    }

    /* cleanup */
    for (unsigned i = 0; i < args[0]; ++i) {
        for (unsigned j = 0; j < args[1]; ++j)
            free(uni[i][j]);

        free(uni[i]);
    }

    free(args);
    free(uni);

    return EXIT_SUCCESS;
}
