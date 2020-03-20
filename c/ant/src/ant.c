#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    if (argc != 4) {
        fprintf(stderr, "usage : %s [height] [width] [iter]\n", basename(argv[0]));

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
    bool **uni = malloc(a[0] * sizeof *uni);

    if (uni == NULL)
        errx(1, "program failed to allocate memory");

    for (i = 0; i < a[0]; ++i) {
        uni[i] = calloc(a[0], sizeof *uni[i]);

        if (uni[i] == NULL)
            errx(1, "program failed to allocate memory");
    }

    unsigned short dir = 0;
    unsigned ant_x = a[0] / 2;
    unsigned ant_y = a[1] / 2;

    /* run langton's ant */
    for (unsigned n = 0; n < a[2]; ++n) {
        printf("P1\n%ld %ld\n", a[1], a[0]);

        for (i = 0; i < a[0]; ++i)
            for (j = 0; j < a[1]; ++j)
                putchar(uni[i][j] + '0');

        dir = ((short)dir + ((uni[ant_x][ant_y] ^= 1) == 0 ? 1 : -1) + 4) % 4;

        ant_x = ((long)ant_x + DIRS[dir][0] + a[0]) % a[0];
        ant_y = ((long)ant_y + DIRS[dir][1] + a[1]) % a[1];
    }

    /* cleanup */
    for (i = 0; i < a[0]; ++i)
        free(uni[i]);

    free(uni);

    return 0;
}
