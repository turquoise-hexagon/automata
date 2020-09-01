#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
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

/*
 * structures
 */
struct item {
    unsigned x;
    unsigned y;
};

/*
 * helper functions
 */
static void
usage(char *name)
{
    fprintf(
        stderr,
        "usage : %s [-whs <number>]\n\n"
        "options :\n"
        "    -w <number>   set width   to <number> (default : 99)\n"
        "    -h <number>   set height  to <number> (default : 99)\n"
        "    -s <number>   set spacing to <number> (default :  2)\n",
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

    if ((ptr = calloc(size, 1)) == NULL)
        errx(1, "failed to allocate memory");

    return ptr;
}

/*
static inline unsigned
wrap(unsigned coord, long offset, unsigned bound)
{
    return ((long)coord + offset + bound) % bound;
}
*/

/*
 * main function
 */
static void
maze(unsigned width, unsigned height, unsigned spacing)
{
    /* init array */
    bool **uni;

    uni = allocate(height * sizeof(*uni));

    for (unsigned i = 0; i < height; ++i)
        uni[i] = allocate(width * sizeof(*uni[i]));

    uni[height / 2][width / 2] = 1;

    /* init list */
    struct item *list;

    list = allocate(height * width / spacing / spacing * sizeof(*list));

    unsigned index = 0;

    list[index].x = height / 2;
    list[index].y =  width / 2;
    ++index;

    /* run maze generation */
    unsigned x;
    unsigned y;
    long new_x;
    long new_y;
    short dir_x;
    short dir_y;
    unsigned tmp;
    unsigned rand_index;

    while (index != 0) {
        printf("P1\n%u %u\n", width, height);

        for (unsigned i = 0; i < height; ++i)
            for (unsigned j = 0; j < width; ++j)
                putchar(uni[i][j] + '0');

        tmp = rand();

        rand_index = tmp % index;
        x = list[rand_index].x;
        y = list[rand_index].y;

        for (unsigned short i = 0; i < 4; ++i) {
            dir_x = DIRS[(tmp + i) % 4][0];
            dir_y = DIRS[(tmp + i) % 4][1];

            new_x = x + spacing * dir_x;
            new_y = y + spacing * dir_y;

            if (
                new_x >= 0 && new_x < height &&
                new_y >= 0 && new_y <  width &&
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

        for (unsigned short i = 0; i <= spacing; ++i)
            uni[x + i * dir_x][y + i * dir_y] = 1;
    }

    /* cleanup */
    for (unsigned i = 0; i < height; ++i)
        free(uni[i]);

    free(list);
    free(uni);
}

int
main(int argc, char **argv)
{
    /* argument parsing */
    unsigned width   = 99;
    unsigned height  = 99;
    unsigned spacing =  2;

    for (int arg; (arg = getopt(argc, argv, ":w:h:s:")) != -1;)
        switch (arg) {
            case 'w': width   = convert_to_number(optarg); break;
            case 'h': height  = convert_to_number(optarg); break;
            case 's': spacing = convert_to_number(optarg); break;
            default :
                usage(argv[0]);
        }

    if (optind < argc)
        usage(argv[0]);

    srand(time(NULL));

    maze(width, height, spacing);

    return 0;
}
