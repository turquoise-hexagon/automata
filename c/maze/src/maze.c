#include <err.h>
#include <stdbool.h>
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

struct item {
    unsigned x;
    unsigned y;
};

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 4)
        usage("[height] [width] [spacing]", argv[0]);

    unsigned *args = argstou(argc, argv);

    /* init array */
    bool **uni = malloc(args[0] * sizeof *uni);

    if (uni == NULL)
        errx(EXIT_FAILURE, "program failed to allocate memory");

    for (unsigned i = 0; i < args[0]; ++i)
        if ((uni[i] = calloc(args[1], sizeof *uni[i])) == NULL)
            errx(EXIT_FAILURE, "program failed to allocate memory");

    uni[args[0] / 2][args[1] / 2] = 1;

    struct item *list = malloc(
        args[0] *
        args[1] /
        args[2] /
        args[2] *
        sizeof *list
    );

    if (list == NULL)
        errx(EXIT_FAILURE, "program failed to allocate memory");

    unsigned index = 0;
    list[index].x = args[0] / 2;
    list[index].y = args[1] / 2;
    ++index;

    /* run maze */
    unsigned x, y;
    long new_x, new_y;
    short dir_x, dir_y;
    unsigned rand_index, tmp;
    srand(time(NULL));

    for (; index != 0;) {
        printf("P1\n%u %u\n", args[1], args[0]);

        for (unsigned i = 0; i < args[0]; ++i)
            for (unsigned j = 0; j < args[1]; ++j)
                putchar(uni[i][j] + '0');

        tmp = rand();

        rand_index = tmp % index;
        x = list[rand_index].x;
        y = list[rand_index].y;

        for (unsigned short i = 0; i < 4; ++i) {
            dir_x = DIRS[(tmp + i) % 4][0];
            dir_y = DIRS[(tmp + i) % 4][1];

            new_x = x + args[2] * dir_x;
            new_y = y + args[2] * dir_y;

            if (
                    new_x >= 0 && new_x < args[0] &&
                    new_y >= 0 && new_y < args[1] &&
                    uni[new_x][new_y] == 0
               )
                goto jump;
        }

        list[rand_index] = list[--index];
        continue;

        jump:
        list[index].x = new_x;
        list[index].y = new_y;
        ++index;

        for (unsigned short i = 1; i <= args[2]; ++i)
            uni[x + i * dir_x][y + i * dir_y] = 1;
    }

    /* cleanup */
    for (unsigned i = 0; i < args[0]; ++i)
        free(uni[i]);

    free(list);
    free(args);
    free(uni);

    return EXIT_SUCCESS;
}
