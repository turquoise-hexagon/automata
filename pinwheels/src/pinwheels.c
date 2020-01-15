#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WRAP(v, i, o, d) \
    v = ((long) i + o + d) % d

const unsigned N = 500;

const int RULES[][17] = {
    {-1, -1,  1, -1, -1, -1, 0, 0, 0, 0, 0, 1, -1, -1, -1, 1, 1},
    {-1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1,  1,  1,  1, 1, 1},
    {-1, -1,  1,  1,  1, -1, 0, 0, 0, 0, 0, 1,  1,  1, -1, 1, 1}
};

const char* COLORS[] = {
    "201 100 126",
    "016 032 041",
    "113 170 197"
};

int
main(int argc, char** argv)
{
    /* argument parsing */
    if (argc != 4)
        errx(1, "usage : %s [height] [width] [init]", argv[0]);

    errno = 0;
    char* ptr;
    long arg[3];

    for (unsigned i = 0; i < 3; i++) {
        arg[i] = strtol(argv[i + 1], &ptr, 10);

        if (errno != 0 || *ptr != 0 || arg[i] < 0)
            errx(1, "invalid parameter");
    }

    /* init arrays */
    short** uni = malloc(arg[0] * sizeof(short*));
    short** cpy = malloc(arg[0] * sizeof(short*));

    if (uni == NULL || cpy == NULL)
        errx(1, "failed to allocate memory");

    for (unsigned i = 0; i < arg[0]; i++) {
        uni[i] = calloc(arg[1], sizeof(short));
        cpy[i] = calloc(arg[1], sizeof(short));

        if (uni[i] == NULL || cpy[i] == NULL)
            errx(1, "failed to allocate memory");
    }

    long x, y;
    srand(time(NULL));

    for (unsigned i = 0; i < arg[2]; i++) {
        jump:
        x = rand() % arg[0];
        y = rand() % arg[1];

        if (uni[x][y] != 0)
            goto jump;

        uni[x][y] = cpy[x][y] = rand() % 2 == 0 ? -1 : 1;
    }

    /* run pinwheels */
    int cpt;

    for (unsigned n = 0; n < N; n++) {
        printf("P3\n%ld %ld\n255\n", arg[1], arg[0]);

        for (unsigned i = 0; i < arg[0]; i++)
            for (unsigned j = 0; j < arg[1]; j++) {
                printf("%s\n", COLORS[uni[i][j] + 1]);

                cpt = 0;

                for (int a = -1; a <= 1; a++)
                    for (int b = -1; b <= 1; b++)
                        if (a != 0 || b != 0) {
                            WRAP(x, i, a, arg[0]);
                            WRAP(y, j, b, arg[1]);

                            cpt += uni[x][y];
                        }

                cpy[i][j] = RULES[uni[i][j] + 1][cpt + 8];
            }

        for (unsigned i = 0; i < arg[0]; i++)
            for (unsigned j = 0; j < arg[1]; j++)
                uni[i][j] = cpy[i][j];
    }

    /* cleanup */
    for (unsigned i = 0; i < arg[0]; i++) {
        free(uni[i]);
        free(cpy[i]);
    }

    free(uni);
    free(cpy);

    return 0;
}
