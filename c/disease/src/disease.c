#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
 * constants
 */
static const short DIRS[4][2] = {
    {-1,  0},
    { 0, -1},
    { 1,  0},
    { 0,  1}
};

static const char *COLORS[3] = {
    "237 238 240",
    "113 170 197",
    "016 032 041"
};

/*
 * helper functions
 */
static void
usage(char *name)
{
    fprintf(
        stderr,
        "usage : %s [-whi <number>]\n\n"
        "options :\n"
        "    -w <number>   set width  to <number> (default : 500)\n"
        "    -h <number>   set height to <number> (default : 500)\n"
        "    -i <number>   iterate <number> times (default : 500)\n",
        basename(name));

    exit(1);
}

static unsigned
convert_to_number(const char *str)
{
    errno = 0;

    char *ptr;
    long number;

    if ((number = strtol(str, &ptr, 10)) < 0 || errno != 0 || *ptr != 0)
        errx(1, "'%s' isn't a valid positive integer", str);

    return number;
}

static void *
allocate(size_t size)
{
    void *ptr;

    if ((ptr = calloc(1, size)) == NULL)
        errx(1, "failed to allocate memory");

    return ptr;
}

static inline unsigned
wrap(unsigned coord, long offset, unsigned bound)
{
    return ((long)coord + offset + bound) % bound;
}

/*
 * main function
 */
static void
disease(unsigned width, unsigned height, unsigned iter)
{
    /* init array */
    unsigned short **uni;

    uni = allocate(height * sizeof(*uni));

    for (unsigned i = 0; i < height; ++i)
        uni[i] = allocate(width * sizeof(*uni[i]));

    uni[height / 2][width / 2] = 1;

    /* run disease propagation */
    unsigned x;
    unsigned y;
    unsigned tmp;

    for (unsigned n = 0; n < iter; ++n) {
        printf("P3\n%u %u\n255\n", width, height);

        for (unsigned i = 0; i < height; ++i)
            for (unsigned j = 0; j < width; ++j) {
                puts(COLORS[uni[i][j]]);

                tmp = rand();

                switch (uni[i][j]) {
                    case 0:
                        if (tmp % 10 == 0)
                            for (unsigned short k = 0; k < 4; ++k) {
                                x = wrap(i, DIRS[k][0], height);
                                y = wrap(j, DIRS[k][1],  width);

                                if (uni[x][y] != 1)
                                    uni[x][y] = 0;
                            }

                        break;
                    case 1:
                        if (tmp % 2 == 0)
                            for (unsigned short k = 0; k < 4; ++k) {
                                x = wrap(i, DIRS[k][0], height);
                                y = wrap(j, DIRS[k][1],  width);

                                if (uni[x][y] == 0)
                                    uni[x][y] = 1;
                            }
                        else if (tmp /  2 %  10 == 0) uni[i][j] = 2;
                        else if (tmp / 20 % 500 == 0) uni[i][j] = 0;
                }
            }
    }

    /* cleanup */
    for (unsigned i = 0; i < height; ++i)
        free(uni[i]);

    free(uni);
}

int
main(int argc, char **argv)
{
    /* argument parsing */
    unsigned width  = 500;
    unsigned height = 500;
    unsigned iter   = 500;

    for (int arg; (arg = getopt(argc, argv, ":w:h:i:")) != -1;)
        switch (arg) {
            case 'w': width  = convert_to_number(optarg); break;
            case 'h': height = convert_to_number(optarg); break;
            case 'i': iter   = convert_to_number(optarg); break;
            default :
                usage(argv[0]);
        }

    if (optind < argc)
        usage(argv[0]);

    srand(time(NULL));

    disease(width, height, iter);

    return 0;
}
