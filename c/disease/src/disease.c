#include <err.h>
#include <errno.h>
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
    if (argc != 3) {
        fprintf(stderr, "usage: %s [height] [width]\n", argv[0]);

        return 1;
    }

    unsigned i, j;

    errno = 0;
    char* ptr;
    long a[2];

    for (i = 0; i < sizeof(a) / sizeof(long); i++) {
        a[i] = strtol(argv[i + 1], &ptr, 10);

        if (errno != 0 || *ptr != 0 || a[i] < 0)
            errx(1, "'%s' isn't a valid parameter", argv[i + 1]);
    }

    /* init array */
    unsigned short** uni = malloc(a[0] * sizeof(unsigned short*));

    if (uni == NULL)
        errx(1, "program failed to allocate memory");

    for (i = 0; i < a[0]; i++) {
        uni[i] = calloc(a[1], sizeof(unsigned short));

        if (uni[i] == NULL)
            errx(1, "program failed to allocate memory");
    }

    uni[a[0] / 2][a[1] / 2] = 1;

    /* run disease */
    short k;
    unsigned x, y, tmp;
    srand(time(NULL));

    for (unsigned n = 0; n < N; n++) {
        printf("P1\n%ld %ld\n", a[1], a[0]);

        for (i = 0; i < a[0]; i++)
            for (j = 0; j < a[1]; j++) {
                tmp = rand();

                switch (uni[i][j]) {
                    case 0 :
                        putchar('0');

                        if (tmp % 10 == 0)
                            for (k = 0; k < 4; k++) {
                                WRAP(x, i, DIRS[k][0], a[0]);
                                WRAP(y, j, DIRS[k][1], a[1]);

                                if (uni[x][y] != 1)
                                    uni[x][y] = 0;
                            }

                        break;
                    case 1 :
                        putchar('0');

                        if (tmp % 2  == 0)
                            for (k = 0; k < 4; k++) {
                                WRAP(x, i, DIRS[k][0], a[0]);
                                WRAP(y, j, DIRS[k][1], a[1]);

                                if (uni[x][y] == 0)
                                    uni[x][y] = 1;
                            }
                        else if (tmp /  2 %  10 == 0) uni[i][j] = 2;
                        else if (tmp / 20 % 500 == 0) uni[i][j] = 0;

                        break;
                    default :
                        putchar('1');
                }
            }
    }

    /* cleanup */
    for (i = 0; i < a[0]; i++)
        free(uni[i]);

    free(uni);
    return 0;
}
