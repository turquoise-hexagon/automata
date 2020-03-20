#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const short DIRS[4][2] = {
    {-1,  0},
    { 0, -1},
    { 1,  0},
    { 0,  1}
};

struct item {
    unsigned x;
    unsigned y;
};

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 4) {
        fprintf(stderr, "usage : %s [height] [width] [spacing]\n", basename(argv[0]));

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

    /* init arrays */
    bool **uni = malloc(a[0] * sizeof *uni);

    if (uni == NULL)
        errx(1, "program failed to allocate memory");

    for (i = 0; i < a[0]; ++i) {
        uni[i] = calloc(a[1], sizeof *uni[i]);

        if (uni[i] == NULL)
            errx(1, "program failed to allocate memory");
    }

    uni[a[0] / 2][a[1] / 2] = true;

    struct item *list = malloc(a[0] * a[1] / a[2] / a[2] * sizeof *list);

    if (list == NULL)
        errx(1, "program failed to allocate memory");

    unsigned index = 0;

    list[index].x = a[0] / 2;
    list[index].y = a[1] / 2;
    ++index;

    /* run maze */
    unsigned x, y;
    long new_x, new_y;
    short dir_x, dir_y;
    unsigned rand_index, tmp;
    srand(time(NULL));

    while (index != 0) {
        printf("P1\n%ld %ld\n", a[1], a[0]);

        for (i = 0; i < a[0]; ++i)
            for (j = 0; j < a[1]; ++j)
                putchar(uni[i][j] + '0');

        tmp = rand();

        rand_index = tmp % index;
        x = list[rand_index].x;
        y = list[rand_index].y;

        for (i = 0; i < 4; ++i) {
            dir_x = DIRS[(tmp + i) % 4][0];
            dir_y = DIRS[(tmp + i) % 4][1];

            new_x = x + a[2] * dir_x;
            new_y = y + a[2] * dir_y;

            if (
                    new_x >= 0 && new_x < a[0] &&
                    new_y >= 0 && new_y < a[1] &&
                    uni[new_x][new_y] == false
               )
                goto jump;
        }

        list[rand_index] = list[--index];
        continue;

        jump:
        list[index].x = new_x;
        list[index].y = new_y;
        ++index;

        for (i = 1; i <= a[2]; ++i)
            uni[x + i * dir_x][y + i * dir_y] = true;
    }

    /* cleanup */
    for (i = 0; i < a[0]; ++i)
        free(uni[i]);

    free(list);
    free(uni);

    return 0;
}
