# 3D Image Formation Project

- Participants : Hatim RIDAOUI & Ayda SHAFI

## Overview

This project aims to read 3D points from an `.off` file, apply geometric transformations (translation and rotation), and project them onto a 2D image plane using perspective projection. The final result is saved as an image.

The project demonstrates basic 3D image formation, including the transformation of 3D points, projection onto a 2D plane, and the creation of an image using pixel coordinates.

## File Structure

- **`main.c`**: The main file that coordinates reading, transforming, projecting, and rendering of 3D points.
- **`imageFormationUtils.h`**: Header file that declares utility functions for reading, transforming, and saving images.
- **`imageFormationUtils.c`**: Implements the utility functions declared in the header file.
- **`Makefile`**: Used to compile and link the project.

---

## main.c

### 1. `int main(int argc, char *argv[])`

- **Purpose**: The main function that drives the entire program.
- **Steps**:
  - Reads the 3D points from an `.off` file.
  - Centers the point cloud around the origin.
  - Applies a rigid transformation (rotation and translation).
  - Projects the 3D points onto a 2D image plane.
  - Converts the 2D image plane coordinates to pixel coordinates.
  - Creates an image from the pixel data and saves it to a `.ppm` file.
- **Key Variables**:
  - `points`: An array of 3D points read from the `.off` file.
  - `f`: The focal length used for perspective projection.
  - `Ximg`, `Yimg`: Arrays for storing the projected 2D coordinates of the points.
  - `u`, `v`: Arrays for storing the pixel coordinates after projection.
  - `image`: A buffer to store pixel values for the final image.
- **Output**: A `PPM` image file (`output.ppm`).

### 2. Key Function Calls in main.c

- **`readOff`**: Reads 3D points from the `.off` file.
- **`centerThePCL`**: Centers the point cloud by translating the centroid of the points to the origin.
- **`rigidTransformation`**: Applies a rigid transformation (rotation and translation) to the points.
- **`perspective_projection`**: Projects the 3D points onto the image plane using perspective projection.
- **`projectToImagePlane`**: Maps the projected 3D points to 2D coordinates on the image plane.
- **`uvProjection`**: Converts the 2D image plane coordinates into pixel coordinates.
- **`saveImage`**: Saves the image in PPM format.

---

## imageFormationUtils.h

### 1. `struct point3d`

- **Purpose**: Structure representing a 3D point.
- **Fields**:
  - `x`, `y`, `z`: Coordinates of the 3D point.
  - `r`, `g`, `b`: Color values for the point.

### 2. Function Declarations

- `struct point3d *readOff(char *fileName, int *N);`
- `void centerThePCL(struct point3d *points, int N);`
- `void rigidTransformation(int N, struct point3d *points, float gamma, float beta, float alpha, float T_x, float T_y, float T_z);`
- `int perspective_projection(int N, struct point3d *points, int f);`
- `void projectToImagePlane(int N, struct point3d *points, int f, float *Ximg, float *Yimg);`
- `void uvProjection(int N, float *Ximg, float *Yimg, float alpha_u, float alpha_v, int u0, int v0, int *u, int *v);`
- `void saveImage(int width, int height, unsigned char *image, const char *filename);`
- `void computeTrans(float gamma, float beta, float alpha, float T_x, float T_y, float T_z, float *result);`
- `void frontFacingPoints(int N, struct point3d *points, float *Ximg, float *Yimg, int *frontFacingIndices, int *count);`

---

## imageFormationUtils.c

### 1. `struct point3d *readOff(char *fileName, int *N)`

- **Purpose**: Reads 3D points from an `.off` file.
- **Parameters**:
  - `fileName`: The name of the `.off` file.
  - `N`: Pointer to an integer to store the number of points.
- **Output**: Returns a pointer to an array of `struct point3d`, representing the 3D point cloud.

### 2. `void centerThePCL(struct point3d *points, int N)`

- **Purpose**: Centers the point cloud by translating the centroid of the points to the origin (0,0,0).
- **Input**: The array of 3D points (`points`) and the number of points (`N`).

### 3. `void rigidTransformation(int N, struct point3d *points, float gamma, float beta, float alpha, float T_x, float T_y, float T_z)`

- **Purpose**: Applies a rigid transformation to the point cloud. This includes both rotation (yaw, pitch, roll) and translation.
- **Input**:
  - `N`: The number of points.
  - `points`: The 3D point array.
  - `gamma, beta, alpha`: Rotation angles (roll, pitch, yaw).
  - `T_x, T_y, T_z`: Translation values along the x, y, and z axes.

### 4. `int perspective_projection(int N, struct point3d *points, int f)`

- **Purpose**: Projects the 3D points onto the 2D image plane using a perspective projection.
- **Input**:
  - `N`: Number of points.
  - `points`: Array of 3D points.
  - `f`: Focal length.
- **Output**: The points are modified to include their projected coordinates.

### 5. `void projectToImagePlane(int N, struct point3d *points, int f, float *Ximg, float *Yimg)`

- **Purpose**: Projects the 3D points to the 2D image plane using perspective projection and stores the results in `Ximg` and `Yimg`.
- **Input**:
  - `N`: Number of points.
  - `points`: Array of 3D points.
  - `Ximg`, `Yimg`: Arrays to store the projected 2D image coordinates.

### 6. `void uvProjection(int N, float *Ximg, float *Yimg, float alpha_u, float alpha_v, int u0, int v0, int *u, int *v)`

- **Purpose**: Converts the 2D image plane coordinates into pixel coordinates (`u`, `v`).
- **Input**:
  - `Ximg`, `Yimg`: 2D image plane coordinates.
  - `alpha_u`, `alpha_v`: Scaling factors for the x and y axes.
  - `u0`, `v0`: The center pixel coordinates.
- **Output**: Fills `u` and `v` arrays with pixel coordinates.

### 7. `void saveImage(int width, int height, unsigned char *image, const char *filename)`

- **Purpose**: Saves the pixel data to a PPM file.
- **Input**:
  - `width`, `height`: Dimensions of the image.
  - `image`: Pixel data.
  - `filename`: Name of the output file.

### 8. `void computeTrans(float gamma, float beta, float alpha, float T_x, float T_y, float T_z, float *result)`

- **Purpose**: Computes a transformation matrix from rotation angles (yaw, pitch, roll) and translation values.
- **Input**:
  - `gamma, beta, alpha`: Rotation angles (roll, pitch, yaw).
  - `T_x, T_y, T_z`: Translation values.
  - `result`: A 1D array of 16 floats that stores the resulting transformation matrix.
- **Example**: This function can be used to compute a 3D transformation matrix and apply it to a point cloud.

### 9. `void frontFacingPoints(int N, struct point3d *points, float *Ximg, float *Yimg, int *frontFacingIndices, int *count)`

- **Purpose**: Filters out front-facing points from the 3D point cloud based on their projected 2D image coordinates.
- **Input**:
  - `N`: Number of points.
  - `points`: Array of 3D points.
  - `Ximg`, `Yimg`: Projected 2D image coordinates.
  - `frontFacingIndices`: Array to store the indices of front-facing points.
  - `count`: Pointer to an integer that will store the number of front-facing points.

---

## Makefile

### Purpose

The `Makefile` compiles the project by linking all the necessary files and ensuring that the final executable is created.

### Commands

- **`all`**: Compiles all the source files and generates the executable.
- **`clean`**: Removes the object files and the executable.

---

## How to Run the Project

1. **Compiling**: Use the `Makefile` by running the following command:

   ```bash
   make
   ```

   This will generate the executable.

   Running: After compiling, run the program by executing:

   ```bash
   ./render
   ```

   Output: The program will generate an output image in PPM format named output.ppm.

## Conclusion

This project showcases the fundamentals of 3D image formation through perspective projection, rigid transformations, and image plane projection. It can be extended for more complex operations such as shading, lighting, or advanced culling techniques.
