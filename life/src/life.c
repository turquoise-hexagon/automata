#include <err.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

const unsigned N = 500;

int
main(int argc, char** argv)
{
    if (argc != 4)
        errx(1, "usage : %s [height] [width] [init]", argv[0]);

    errno = 0;
    long arg[3];
    char* ptr = NULL;

    for (unsigned i = 0; i < 3; i++) {
        arg[i] = strtol(argv[i + 1], & ptr, 10);

        if (errno != 0 || * ptr != 0 || arg[i] < 0)
            errx(1, "invalid parameter");
    }

    unsigned** uni = malloc(arg[0] * sizeof(unsigned*));
    unsigned** cpy = malloc(arg[0] * sizeof(unsigned*));

    if (!uni || !cpy)
        errx(1, "failed to allocate memory");

    for (unsigned i = 0; i < arg[0]; i++) {
        uni[i] = calloc(arg[1], sizeof(unsigned));
        cpy[i] = calloc(arg[1], sizeof(unsigned));

        if (!uni[i] || !cpy[i])
            errx(1, "failed to allocate memory");
    }

    long x, y;
    for (unsigned i = 0; i < arg[2]; i++) {
        jump:
        x = rand() % arg[0];
        y = rand() % arg[1];

        if (uni[x][y] != 0)
            goto jump;

        uni[x][y] = 1;
    }

    unsigned cpt;
    for (unsigned n = 0; n < N; n++) {
        printf("P1\n%ld %ld\n", arg[1], arg[0]);

        for (unsigned i = 0; i < arg[0]; i++)
            for (unsigned j = 0; j < arg[1]; j++) {
                putchar(uni[i][j] == 0 ? '0' : '1');

                cpt = 0;

                for (int a = -1; a <= 1; a++)
                    for (int b = -1; b <= 1; b++)
                        if (a != 0 || b != 0) {
                            x = i + a, x = x < 0 ? arg[0] - 1 : x % arg[0];
                            y = j + b, y = y < 0 ? arg[1] - 1 : y % arg[1];

                            if (uni[x][y] == 1)
                                cpt++;
                        }

                switch (cpt) {
                    case 2  :                break;
                    case 3  : cpy[i][j] = 1; break;
                    default : cpy[i][j] = 0;
                }
            }

        putchar('\n');

        for (unsigned i = 0; i < arg[0]; i++)
            for (unsigned j = 0; j < arg[1]; j++)
                uni[i][j] = cpy[i][j];
    }

    for (unsigned i = 0; i < arg[0]; i++) {
        free(uni[i]);
        free(cpy[i]);
    }

    free(uni);
    free(cpy);

    return 0;
}
