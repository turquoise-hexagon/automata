#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WRAP(v, i, o, d) \
    v = ((long)i + o + d) % d

static const char *COLORS[] = {
    "016 032 041",
    "113 170 197",
    "237 238 240"
};

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 5) {
        fprintf(stderr, "usage : %s [height] [width] [init] [iter]\n", basename(argv[0]));

        return 1;
    }

    unsigned i, j;

    errno = 0;
    char *ptr;
    long a[4];

    for (i = 0; i < sizeof(a) / sizeof(long); i++) {
        a[i] = strtol(argv[i + 1], &ptr, 10);

        if (errno != 0 || *ptr != 0 || a[i] < 0)
            errx(1, "'%s' isn't a valid parameter", argv[i + 1]);
    }

    /* init array */
    unsigned short ***uni = malloc(a[0] * sizeof(unsigned short **));
    
    if (uni == NULL)
        errx(1, "program failed to allocate memory");

    for (i = 0; i < a[0]; i++) {
        uni[i] = malloc(a[1] * sizeof(unsigned short *));

        if (uni[i] == NULL)
            errx(1, "program failed to allocate memory");

        for (j = 0; j < a[1]; j++) {
            uni[i][j] = calloc(2, sizeof(unsigned short));

            if (uni[i][j] == NULL)
                errx(1, "program failed to allocate memory");
        }
    }

    unsigned x, y;
    srand(time(NULL));

    for (i = 0; i < a[2]; i++) {
        jump:
        x = rand() % a[0];
        y = rand() % a[1];

        if (uni[x][y][0] != 0)
            goto jump;

        uni[x][y][0] = rand() % 2 + 1;
    }

    /* run brian's brain */
    bool flag = false;
    unsigned short tmp = 0, cnt;

    for (unsigned n = 0; n < a[3]; n++) {
        printf("P3\n%ld %ld\n255\n", a[1], a[0]);

        for (i = 0; i < a[0]; i++)
            for (j = 0; j < a[1]; j++) {
                puts(COLORS[uni[i][j][flag]]);

                switch (uni[i][j][flag]) {
                    case 0  :
                        cnt = 0;

                        for (short u = -1; u <= 1; u++)
                            for (short v = -1; v <= 1; v++)
                                if (u != 0 || v != 0) {
                                    WRAP(x, i, u, a[0]);
                                    WRAP(y, j, v, a[1]);

                                    if (uni[x][y][flag] == 2)
                                        cnt++;
                                }

                        tmp = cnt == 2 ? 2 : 0;

                        break;
                    case 1  :
                        tmp = 0;

                        break;
                    default : 
                        tmp = 1;
                }

                uni[i][j][!flag] = tmp;
            }

        flag ^= true;
    }

    /* cleanup */
    for (i = 0; i < a[0]; i++) {
        for (j = 0; j < a[1]; j++)
            free(uni[i][j]);

        free(uni[i]);
    }

    free(uni);

    return 0;
}
