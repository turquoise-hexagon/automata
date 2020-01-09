#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WRAP(v, i, o, d) \
    v = (long) i + o, v = v < 0 ? d - 1 : v % d

const unsigned N = 500;

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
    bool** uni = malloc(arg[0] * sizeof(bool*));
    bool** cpy = malloc(arg[0] * sizeof(bool*));

    if (uni == NULL || cpy == NULL)
        errx(1, "failed to allocate memory");

    for (unsigned i = 0; i < arg[0]; i++) {
        uni[i] = calloc(arg[1], sizeof(bool));
        cpy[i] = calloc(arg[1], sizeof(bool));

        if (uni[i] == NULL || cpy[i] == NULL)
            errx(1, "failed to allocate memory");
    }

    long x, y;

    for (unsigned i = 0; i < arg[2]; i++) {
        jump:
        x = rand() % arg[0];
        y = rand() % arg[1];

        if (uni[x][y] == true)
            goto jump;

        uni[x][y] = true;
    }

    /* run the game of life */
    unsigned cpt;

    for (unsigned n = 0; n < N; n++) {
        printf("P1\n%ld %ld\n", arg[1], arg[0]);

        for (unsigned i = 0; i < arg[0]; i++)
            for (unsigned j = 0; j < arg[1]; j++) {
                putchar(uni[i][j] == false ? '0' : '1');

                cpt = 0;

                for (int a = -1; a <= 1; a++)
                    for (int b = -1; b <= 1; b++)
                        if (a != 0 || b != 0) {
                            WRAP(x, i, a, arg[0]);
                            WRAP(y, j, b, arg[1]);

                            if (uni[x][y] == true)
                                cpt++;
                        }

                switch (cpt) {
                    case 2  : /* do nothing */  break;
                    case 3  : cpy[i][j] = true; break;
                    default : cpy[i][j] = false;
                }
            }

        putchar('\n');

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
