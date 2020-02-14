#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WRAP(v, i, o, d) \
    v = ((long) i + o + d) % d

const unsigned N = 500;

const short DIRS[][2] = {
    {-1,  0},
    { 0, -1},
    { 1,  0},
    { 0,  1}
};

int
main(int argc, char** argv)
{
    /* argument parsing */
    if (argc != 3)
        errx(1, "usage : %s [height] [width]", argv[0]);

    unsigned i, j, k;

    errno = 0;
    char* ptr;
    long arg[2];

    for (i = 0; i < sizeof(arg) / sizeof(long); i++) {
        arg[i] = strtol(argv[i + 1], &ptr, 10);

        if (errno != 0 || *ptr != 0 || arg[i] < 0)
            errx(1, "invalid parameter");
    }

    /* init array */
    unsigned short** uni = malloc(arg[0] * sizeof(unsigned short*));

    if (uni == NULL)
        errx(1, "failed to allocate memory");

    for (i = 0; i < arg[0]; i++) {
        uni[i] = calloc(arg[1], sizeof(unsigned short));

        if (uni[i] == NULL)
            errx(1, "failed to allocate memory");
    }

    uni[arg[0] / 2][arg[1] / 2] = 1;

    long x, y;
    unsigned tmp;
    unsigned short bin = 0;
    srand(time(NULL));

    /* run disease */
    for (unsigned n = 0; n < N; n++) {
        printf("P4\n%ld %ld\n", arg[1], arg[0]);

        for (i = 0; i < arg[0]; i++)
            for (j = 0; j < arg[1] + arg[1] % 8; j++) {
                bin <<= true;

                if (j < arg[1]) {
                    tmp = rand();

                    switch (uni[i][j]) {
                        case 0 :
                            bin |= false;

                            if (tmp % 10 == 0)
                                for (k = 0; k < 4; k++) {
                                    WRAP(x, i, DIRS[k][0], arg[0]);
                                    WRAP(y, j, DIRS[k][1], arg[1]);

                                    if (uni[x][y] != 1)
                                        uni[x][y] = 0;
                                }

                            break;
                        case 1 :
                            bin |= false;

                            if (tmp % 2  == 0)
                                for (k = 0; k < 4; k++) {
                                    WRAP(x, i, DIRS[k][0], arg[0]);
                                    WRAP(y, j, DIRS[k][1], arg[1]);

                                    if (uni[x][y] == 0)
                                        uni[x][y] = 1;
                                }
                            else if (tmp / 2  %  10 == 0) uni[i][j] = 2;
                            else if (tmp / 20 % 500 == 0) uni[i][j] = 0;

                            break;
                        default :
                            bin |= true;
                    }
                }
                else
                    bin |= true;

                if (j % 8 == 7)
                    putchar(bin);
            }
    }

    /* cleanup */
    for (i = 0; i < arg[0]; i++)
        free(uni[i]);

    free(uni);

    return 0;
}
