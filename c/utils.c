#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void
usage(char *msg, char *name)
{
    fprintf(stderr, "usage : %s %s\n", basename(name), msg);

    exit(1);
}

unsigned *
argstous(int argc, char **argv)
{
    long tmp;
    char *ptr;
    errno = 0;

    unsigned *array = malloc((argc - 1) * sizeof *array);

    if (array == NULL)
        errx(1, "program failed to allocate memory");

    for (unsigned i = 0; i < (unsigned)argc - 1; ++i) {
        tmp = strtol(argv[i + 1], &ptr, 10);

        if (errno != 0 || *ptr != 0 || tmp < 0)
            errx(1, "'%s' isn't a valid positive integer", argv[i + 1]);

        array[i] = (unsigned)tmp;
    }

    return array;
}

