#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WRAP(v, i, o, d) \
    v = ((long)i + o + d) % d

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
    if (argc != 4) {
        fprintf(stderr, "usage: %s [height] [width] [iter]\n", basename(argv[0]));

        return 1;
    }

    unsigned i, j;

    errno = 0;
    char *ptr;
    long a[3];

    for (i = 0; i < sizeof(a) / sizeof(long); ++i) {
        a[i] = strtol(argv[i + 1], &ptr, 10);

        if (errno != 0 || *ptr != 0 || a[i] < 0)
            errx(1, "'%s' isn't a valid positive integer", argv[i + 1]);
    }

    /* init array */
    unsigned short **uni = malloc(a[0] * sizeof(unsigned short *));

    if (uni == NULL)
        errx(1, "program failed to allocate memory");

    for (i = 0; i < a[0]; ++i) {
        uni[i] = calloc(a[1], sizeof(unsigned short));

        if (uni[i] == NULL)
            errx(1, "program failed to allocate memory");
    }

    uni[a[0] / 2][a[1] / 2] = 1;

    /* run disease */
    unsigned short k;
    unsigned x, y, tmp;
    srand(time(NULL));

    for (unsigned n = 0; n < a[2]; ++n) {
        printf("P3\n%ld %ld\n255\n", a[1], a[0]);

        for (i = 0; i < a[0]; ++i)
            for (j = 0; j < a[1]; ++j) {
                tmp = rand();

                puts(COLORS[uni[i][j]]);

                switch (uni[i][j]) {
                    case 0 :
                        if (tmp % 10 == 0)
                            for (k = 0; k < 4; ++k) {
                                WRAP(x, i, DIRS[k][0], a[0]);
                                WRAP(y, j, DIRS[k][1], a[1]);

                                if (uni[x][y] != 1)
                                    uni[x][y] = 0;
                            }

                        break;
                    case 1 :
                        if (tmp % 2  == 0)
                            for (k = 0; k < 4; ++k) {
                                WRAP(x, i, DIRS[k][0], a[0]);
                                WRAP(y, j, DIRS[k][1], a[1]);

                                if (uni[x][y] == 0)
                                    uni[x][y] = 1;
                            }
                        else if (tmp /  2 %  10 == 0) uni[i][j] = 2;
                        else if (tmp / 20 % 500 == 0) uni[i][j] = 0;

                        break;
                }
            }
    }

    /* cleanup */
    for (i = 0; i < a[0]; ++i)
        free(uni[i]);

    free(uni);

    return 0;
}
