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

    // Centering the PCL
    /* centerThePCL(points, N_v); */

    int f = 1.5;

    // Applying a rigid transformation, replacing p by p' (1)

    float yaw = 0.2f;
    float pitch = 0.2f;
    float roll = 0.2f;
    float Tx = 0.2f;
    float Ty = 0.2f;
    float Tz = 0.2f;

    // Translation calculation debug
    /* for (int i = N_v - 10; i < N_v; i++)
    {
        float originalX = points[i].x;
        float originalY = points[i].y;
        float originalZ = points[i].z;

        printf("Original Point %d: X = %f, Y = %f, Z = %f\n", i, originalX, originalY, originalZ);

        rigidTransformation(1, &points[i], yaw, pitch, roll, Tx, Ty, Tz);

        printf("Transformed Point %d: X = %f, Y = %f, Z = %f\n\n", i, points[i].x, points[i].y, points[i].z);
    } */

    rigidTransformation(N_v, points, yaw, pitch, roll, Tx, Ty, Tz);

    /* // Debug print transformed points
    for (int i = 0; i < N_v; i++)
    {
        printf("Transformed Point %d: X = %f, Y = %f, Z = %f\n", i, points[i].x, points[i].y, points[i].z);
    } */

    // (2) projection

    perspective_projection(N_v, points, f);

    float *Ximg = (float *)malloc(N_v * sizeof(float));
    float *Yimg = (float *)malloc(N_v * sizeof(float));

    if (Ximg == NULL || Yimg == NULL)
    {
        printf("Memory allocation for image plane coordinates failed.\n");
        free(points);
        return EXIT_FAILURE;
    }

    // (3) projection
    projectToImagePlane(N_v, points, f, Ximg, Yimg);

    /*  for (int i = 0; i < N_v; i++)
     {
         printf("Point %d: Ximg = %f, Yimg = %f\n", i, Ximg[i], Yimg[i]);
     } */

    float alpha_u = 0.001;
    float alpha_v = 0.001;
    int u0 = 512;
    int v0 = 512;

    int *u = (int *)malloc(N_v * sizeof(int));
    int *v = (int *)malloc(N_v * sizeof(int));

    if (u == NULL || v == NULL)
    {
        printf("Memory allocation for pixel coordinates failed.\n");
        free(points);
        free(Ximg);
        free(Yimg);
        return EXIT_FAILURE;
    }

    // UV pixel projection
    uvProjection(N_v, Ximg, Yimg, alpha_u, alpha_v, u0, v0, u, v);

    /*  for (int i = 0; i < N_v; i++)
     {
         printf("Point %d: u = %d, v = %d\n", i, u[i], v[i]);
     } */

    int imageWidth = 1024, imageHeight = 1024;
    unsigned char *image = (unsigned char *)calloc(imageWidth * imageHeight * 3, sizeof(unsigned char));
    if (image == NULL)
    {
        printf("Memory allocation for image failed.\n");
        free(Ximg);
        free(Yimg);
        free(u);
        free(v);
        free(points);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < N_v; i++)
    {
        if (u[i] >= 0 && u[i] < imageWidth && v[i] >= 0 && v[i] < imageHeight)
        {
            int index = (v[i] * imageWidth + u[i]) * 3;
            image[index] = points[i].r;     // Red
            image[index + 1] = points[i].g; // Green
            image[index + 2] = points[i].b; // Blue
        }
    }

    saveImage(imageWidth, imageHeight, image, "output.ppm");

    free(Ximg);
    free(Yimg);
    free(u);
    free(v);
    free(points);

    return 0;
}
