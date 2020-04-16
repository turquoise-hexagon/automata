#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

static const short DIRS[][2] = {
    {-1,  0},
    { 0, -1},
    { 1,  0},
    { 0,  1}
};

static const char *COLORS[] = {
    "237 238 240",
    "113 170 197",
    "016 032 041"
};

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 4)
        usage("[height] [width] [iter]", argv[0]);

    unsigned *args = argstous(argc, argv);

    /* init array */
    unsigned short **uni = malloc(args[0] * sizeof *uni);

    if (uni == NULL)
        errx(EXIT_FAILURE, "program failed to allocate memory");

    for (unsigned i = 0; i < args[0]; ++i)
        if ((uni[i] = calloc(args[1], sizeof *uni[i])) == NULL)
            errx(EXIT_FAILURE, "program failed to allocate memory");

    uni[args[0] / 2][args[1] / 2] = 1;

    /* run disease */
    unsigned tmp;
    unsigned x, y;
    srand(time(NULL));

    for (unsigned n = 0; n < args[2]; ++n) {
        printf("P3\n%u %u\n255\n", args[1], args[0]);

        for (unsigned i = 0; i < args[0]; ++i)
            for (unsigned j = 0; j < args[1]; ++j) {
                tmp = rand();

                puts(COLORS[uni[i][j]]);

                switch (uni[i][j]) {
                    case 0:
                        if (tmp % 10 == 0)
                            for (unsigned short k = 0; k < 4; ++k) {
                                x = ((long)i + DIRS[k][0] + args[0]) % args[0];
                                y = ((long)j + DIRS[k][1] + args[1]) % args[1];

                                if (uni[x][y] != 1)
                                    uni[x][y] = 0;
                            }

                        break;
                    case 1:
                        if (tmp % 2 == 0)
                            for (unsigned short k = 0; k < 4; ++k) {
                                x = ((long)i + DIRS[k][0] + args[0]) % args[0];
                                y = ((long)j + DIRS[k][1] + args[1]) % args[1];

                                if (uni[x][y] == 0)
                                    uni[x][y] = 1;
                            }
                        else if (tmp /  2 %  10 == 0) uni[i][j] = 2;
                        else if (tmp / 20 % 500 == 0) uni[i][j] = 0;
                }
            }
    }

    /* cleanup */
    for (unsigned i = 0; i < args[0]; ++i)
        free(uni[i]);

    free(args);
    free(uni);

    return EXIT_SUCCESS;
}
