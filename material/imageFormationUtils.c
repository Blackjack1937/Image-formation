/* Naive off reader for point cloud from .off file */
/* Only works for TP5 */
/* Point cloud is stored in points */
/* The size of points is N */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "imageFormationUtils.h"

struct point3d *readOff(char *fileName, int *N)
{
  struct point3d *points;

  FILE *ifp = fopen(fileName, "r");
  if (ifp == NULL)
  {
    fprintf(stderr, "Can't open input file in.list!\n");
    exit(1);
  }

  char fooChar[16];
  int N_v, fooInt;
  fscanf(ifp, "%s", fooChar);
  // printf("%s \n", fooChar);
  fscanf(ifp, "%d %d %d", &N_v, &fooInt, &fooInt);
  *N = N_v;
  points = (struct point3d *)malloc(N_v * sizeof(struct point3d));
  // printf("%d %d %d\n", N_v, fooInt, fooInt);
  int i = 0;
  for (i = 0; i < N_v; i++)
  {
    fscanf(ifp, "%f %f %f %d %d %d %d", &(points[i].x), &(points[i].y), &(points[i].z), &(points[i].r), &(points[i].g), &(points[i].b), &fooInt);
    // printf("%f %f %f %d %d %d\n", points[i].x, points[i].y, points[i].z, points[i].r, points[i].g, points[i].b);
  }

  return points;
}

void centerThePCL(struct point3d *points, int N)
{
  int i = 0;
  float xMin = 100000, xMax = -1000000, yMin = 1000000, yMax = -10000000, zMin = 100000000, zMax = -10000000;
  for (i = 0; i < N; i++)
  {
    if (points[i].x > xMax)
      xMax = points[i].x;
    else if (points[i].x < xMin)
      xMin = points[i].x;

    if (points[i].y > yMax)
      yMax = points[i].y;
    else if (points[i].y < yMin)
      yMin = points[i].y;

    if (points[i].z > zMax)
      zMax = points[i].z;
    else if (points[i].z < zMin)
      zMin = points[i].z;
  }

  float xMid = (xMin + xMax) / 2, yMid = (yMin + yMax) / 2, zMid = (zMin + zMax) / 2;
  for (i = 0; i < N; i++)
  {
    points[i].x = points[i].x - xMid;
    points[i].y = points[i].y - yMid;
    points[i].z = points[i].z - zMid;
  }
}

void Rx(double alpha, float *R)
{

  int i = 0, j = 0;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      R[i * 4 + j] = 0;
  // 1      0     0    0
  //   0     cosA -sinA  0
  //   0     sinA  cosA  0
  //   0      0     0    1

  R[0] = 1;

  R[5] = cos(alpha);
  R[6] = -sin(alpha);

  R[9] = sin(alpha);
  R[10] = cos(alpha);

  R[15] = 1;

  return;
}

void Ry(float alpha, float *R)
{

  int i = 0, j = 0;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      R[i * 4 + j] = 0;
  /* cosA    0    sinA   0
      0      1     0     0
    -sinA    0    cosA   0
      0      0     0     1
      */
  R[0] = cos(alpha);
  R[2] = sin(alpha);

  R[5] = 1;

  R[8] = -sin(alpha);
  R[10] = cos(alpha);

  R[15] = 1;

  return;
}

void Rz(float alpha, float *R)
{

  int i = 0, j = 0;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      R[i * 4 + j] = 0;
  /* cosA  -sinA   0   0
     sinA   cosA   0   0
      0      0     1   0
      0      0     0   1
      */
  R[0] = cos(alpha);
  R[1] = -sin(alpha);

  R[4] = sin(alpha);
  R[5] = cos(alpha);

  R[10] = 1;
  R[15] = 1;

  return;
}

void matMul(float *A, float *B, float *result)
{
  float tmpA[16], tmpB[16];
  int i = 0, j = 0;
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      tmpA[i * 4 + j] = A[i * 4 + j];
      tmpB[i * 4 + j] = B[i * 4 + j];
    }
  }
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      result[i * 4 + j] = tmpA[i * 4 + 0] * tmpB[0 * 4 + j] + tmpA[i * 4 + 1] * tmpB[1 * 4 + j] + tmpA[i * 4 + 2] * tmpB[2 * 4 + j] + tmpA[i * 4 + 3] * tmpB[3 * 4 + j];
    }
  }
  return;
}

void computeTrans(float gama, float beta, float alpha, float T_x, float T_y, float T_z, float *result)
{
  float R_x[16], R_y[16], R_z[16];
  Rx(gama, R_x);
  Ry(beta, R_y);
  Rz(alpha, R_z);

  matMul(R_y, R_z, result);
  matMul(R_x, result, result);
  result[3] = T_x;
  result[7] = T_y;
  result[11] = T_z;
  return;
}

int perspective_projection(int N, struct point3d *points, int f)
{

  float **array = (float **)malloc(N * sizeof(float *));

  if (array == NULL)
  {
    printf("Memory allocation failed!\n");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < N; i++)
  {
    array[i] = (float *)malloc(4 * sizeof(float));
    if (array[i] == NULL)
    {
      printf("Memory allocation for vector %d failed!\n", i);
      return EXIT_FAILURE;
    }
  }

  for (int i = 0; i < N; i++)
  {
    array[i][0] = points[i].x;
    array[i][1] = points[i].y;
    array[i][2] = 0;
    array[i][3] = 1 + (points[i].z / f);
  }

  // Printing the (2)
  /* for (int i = 0; i < N; i++)
  {
    printf("Column vector %d: ", i);
    for (int j = 0; j < 4; j++)
    {
      printf("%f ", array[i][j]);
    }
    printf("\n");
  } */

  for (int i = 0; i < N; i++)
  {
    free(array[i]);
  }
  free(array);

  return 0;
}

void projectToImagePlane(int N, struct point3d *points, int f, float *Ximg, float *Yimg)
{
  for (int i = 0; i < N; i++)
  {

    float denominator = 1 + (points[i].z / f);

    Ximg[i] = points[i].x / denominator;
    Yimg[i] = points[i].y / denominator;
  }
}

void uvProjection(int N, float *Ximg, float *Yimg, float alpha_u, float alpha_v, int u0, int v0, int *u, int *v)
{
  for (int i = 0; i < N; i++)
  {

    u[i] = (int)(Ximg[i] / alpha_u + u0);
    v[i] = (int)(Yimg[i] / alpha_v + v0);
  }
}

// Creating the image output using u and v

void saveImage(int width, int height, unsigned char *image, const char *filename)
{
  FILE *fp = fopen(filename, "wb");
  if (!fp)
  {
    printf("Unable to open file for writing: %s\n", filename);
    return;
  }
  fprintf(fp, "P6\n%d %d\n255\n", width, height); // Magic number
  fwrite(image, sizeof(unsigned char), width * height * 3, fp);
  fclose(fp);
}

void rigidTransformation(int N, struct point3d *points, float gamma, float beta, float alpha, float T_x, float T_y, float T_z)
{
  float transmatrix[16];
  computeTrans(gamma, beta, alpha, T_x, T_y, T_z, transmatrix);
  for (int i = 0; i < N; i++)
  {
    float x = points[i].x;
    float y = points[i].y;
    float z = points[i].z;
    points[i].x = transmatrix[0] * x + transmatrix[1] * y + transmatrix[2] * z + transmatrix[3];
    points[i].y = transmatrix[4] * x + transmatrix[5] * y + transmatrix[6] * z + transmatrix[7];
    points[i].z = transmatrix[8] * x + transmatrix[9] * y + transmatrix[10] * z + transmatrix[11];
  }
}

// Orthogonal projection

void orthogonalProjection(int N, struct point3d *points, float *Ximg, float *Yimg)
{
  for (int i = 0; i < N; i++)
  {
    Ximg[i] = points[i].x;
    Yimg[i] = points[i].y;
  }
}
