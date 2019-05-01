#include <err.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N 3000

int
main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage : %s [rule] [bits]\n", argv [0]);

        return 1;
    }

    /* initialize */
    for (unsigned int i = 0; argv [1] [i]; i++)
        if (! isdigit(argv [1] [i]))
            errx(1, "invalid rule");

    unsigned int r =   atoi(argv [1]);
    unsigned int l = strlen(argv [2]);

    bool* o = malloc((l + 2) * sizeof(bool));
    bool* b = malloc((l + 2) * sizeof(bool));

    if (! o || ! b)
        errx(1, "failed to malloc");

    o [0] = o [l] = 0;
    b [0] = b [l] = 0;

    for (unsigned int i = 1; i <= l; i++) {
        switch (argv [2] [i - 1]) {
            case '0' : o [i] = 0; break;
            case '1' : o [i] = 1; break;
            default : errx(1, "invalid strip");
        }
    }

    /* run automaton */
    for (unsigned int n = 0; n < N; n++) {
        for (unsigned int t = 0, i = 1; i <= l; i++, t = 0) {
            putchar(o [i] ? '1' : '0');

            for (int j = -1; j < 2; j++)
                t = t << 1 | o [i + j];

            b [i] = 1 & r >> t;
        }

        putchar('\n');

        for (unsigned int i = 1; i <= l; i++)
            o [i] = b [i];
    }

    /* cleanup */
    free(o);
    free(b);

    return 0;
}
