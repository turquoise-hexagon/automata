#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int
main(int argc, char** argv)
{
    if (argc != 3)
        errx(1, "usage : %s[rule][bits]", argv[0]);

    errno = 0;

    /* initialize */
    char* ptr = NULL;

    long int rule = strtol(argv[1], & ptr, 10);

    if (errno || * ptr || rule < 0 || rule > 255)
        errx(1, "invalid rule");

    unsigned int length = strlen(argv[2]);

    bool* origin = malloc((length + 2) * sizeof(bool));
    bool* backup = malloc((length + 2) * sizeof(bool));

    if (!origin || !backup)
        errx(1, "failed to malloc");

    origin[0] = origin[length] = 0;
    backup[0] = backup[length] = 0;

    for (unsigned int i = 1; i <= length; i++)
        switch (argv[2][i - 1]) {
            case '0' : origin[i] = 0; break;
            case '1' : origin[i] = 1; break;
            default  :
                free(origin);
                free(backup);

                errx(1, "invalid strip");
        }

    printf("P1\n%d %d\n", length, length);

    for (unsigned int n = 0; n < length; n++) {
        for (unsigned int tmp = 0, i = 1; i <= length; tmp = 0, i++) {
            putchar(origin[i] ? '1' : '0');

            for (int j = -1; j < 2; j++)
                tmp = tmp << 1 | origin[i + j];

            backup[i] = 1 & rule >> tmp;
        }

        putchar('\n');

        for (unsigned int i = 1; i <= length; i++)
            origin[i] = backup[i];
    }

    /* cleanup */
    free(origin);
    free(backup);

    return 0;
}
