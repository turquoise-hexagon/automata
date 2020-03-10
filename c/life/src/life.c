#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WRAP(v, i, o, d) \
    v = ((long)i + o + d) % d

static const unsigned N = 500;

int
main(int argc, char** argv)
{
    /* argument parsing */
    if (argc != 4) {
        fprintf(stderr, "usage: %s [height] [width] [init]\n", basename(argv[0]));

        return 1;
    }

    unsigned i, j;

    errno = 0;
    char* ptr;
    long a[3];

    for (i = 0; i < sizeof(a) / sizeof(long); i++) {
        a[i] = strtol(argv[i + 1], &ptr, 10);

        if (errno != 0 || *ptr != 0 || a[i] < 0)
            errx(1, "'%s' isn't a valid parameter", argv[i + 1]);
    }

    /* init array */
    bool*** gol = malloc(a[0] * sizeof(bool**));

    if (gol == NULL)
        errx(1, "program failed to allocate memory");

    for (i = 0; i < a[0]; i++) {
        gol[i] = malloc(a[1] * sizeof(bool*));

        if (gol[i] == NULL)
            errx(1, "program failed to allocate memory");

        for (j = 0; j < a[1]; j++) {
            gol[i][j] = calloc(2, sizeof(bool));

            if (gol[i][j] == NULL)
                errx(1, "program failed to allocate memory");
        }
    }

    unsigned x, y;
    srand(time(NULL));

    for (i = 0; i < a[2]; i++) {
        jump:
        x = rand() % a[0];
        y = rand() % a[1];

        if (gol[x][y][0] == true)
            goto jump;

        gol[x][y][0] = true;
    }

    /* run the game of life */
    unsigned short cpt;
    bool flag = false, tmp;

    for (unsigned n = 0; n < N; n++) {
        printf("P1\n%ld %ld\n", a[1], a[0]);

        for (i = 0; i < a[0]; i++)
            for (j = 0; j < a[1]; j++) {
                putchar(gol[i][j][flag] == true ? '1' : '0');

                cpt = 0;

                for (short u = -1; u <= 1; u++)
                    for (short v = -1; v <= 1; v++)
                        if (u != 0 || v != 0) {
                            WRAP(x, i, u, a[0]);
                            WRAP(y, j, v, a[1]);

                            if (gol[x][y][flag] == true)
                                cpt++;
                        }

                switch (cpt) {
                    case 2  : tmp = gol[i][j][flag]; break;
                    case 3  : tmp = true; break;
                    default : tmp = false;
                }

                gol[i][j][!flag] = tmp;
            }

        flag ^= true;
    }

    /* cleanup */
    for (i = 0; i < a[0]; i++) {
        for (j = 0; j < a[1]; j++)
            free(gol[i][j]);

        free(gol[i]);
    }

    free(gol);

    return 0;
}
