#include <stdio.h>
#include <stdlib.h>
#include "imageFormationUtils.h"

int main(int argc, char *argv[])
{
    struct point3d *points;
    int N_v = 0;
    points = readOff("frameCube.off", &N_v);

    // centerThePCL(points, N_v);

    perspective_projection(N_v, points, 1);
    printf("The number of points in the .off file are : %d", N_v);

    return 0;
}