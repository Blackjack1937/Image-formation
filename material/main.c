#include <stdio.h>
#include <stdlib.h>
#include "imageFormationUtils.h"

int main(int argc, char *argv[])
{
    struct point3d *points;
    int N_v = 0;
    points = readOff("frameCube.off", &N_v);

    if (points == NULL)
    {
        fprintf(stderr, "Failed to read points from file.\n");
        return 1;
    }

    int f = 1;
    // centerThePCL(points, N_v);
    perspective_projection(N_v, points, f);

    free(points);
    return 0;
}