#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define X 99
#define Y X
#define S 2

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
main(void)
{
    /* init arrays */
    static bool world[X][Y];

    world[X / 2][Y / 2] = true;

    unsigned index = 0;
    static struct item list[X * Y / S / S];

    list[index].x = X / 2;
    list[index].y = Y / 2;
    ++index;

    srand(time(NULL));

    /* run maze */
    long nx, ny;
    short dx, dy;
    unsigned x, y, tmp, tmp_rand;

    while (index != 0) {
        printf("P1\n%u %u\n", X, Y);

        for (unsigned i = 0; i < X; ++i)
            for (unsigned j = 0; j < Y; ++j)
                putchar(world[i][j] ? '1' : '0');

        putchar('\n');

        tmp = rand() % index;
        x = list[tmp].x;
        y = list[tmp].y;

        tmp_rand = rand();

        for (unsigned short i = 0; i < 4; ++i) {
            dx = DIRS[(tmp_rand + i) % 4][0];
            dy = DIRS[(tmp_rand + i) % 4][1];

            nx = x + S * dx;
            ny = y + S * dy;

            if (
                    nx >= 0 && nx < X &&
                    ny >= 0 && ny < Y &&
                    world[nx][ny] == false
               )
                goto jump;
        }

        --index;
        list[tmp] = list[index];
        continue;

        jump:
        list[index].x = nx;
        list[index].y = ny;
        ++index;

        for (unsigned i = 1; i <= S; ++i)
            world[x + i * dx][y + i * dy] = true;
    }

    return 0;
}
