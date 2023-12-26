/**
 *
 * CENG342 Project-1
 *
 * Downscaling
 *
 * Usage:  main <input.jpg> <output.jpg>
 *
 * @group_id 00
 * @author  your names
 *
 * @version 1.0, 02 April 2022
 */

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../include/bilinear_scaling.h"
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#define CHANNEL_NUM 1

int main(int argc, char *argv[]) {
  uint8_t *image;
  Pixel *scaled_image;
  int rank, size, width, height, bpp, new_width, new_height;
  float scale_x, scale_y;
  double time1, time2;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc < 3) {
    printf("Usage: (mpirun -n [number_of_cores]) %s [image_filename] "
           "[output_filename]\n",
           argv[0]);
    MPI_Finalize();
    return 1;
  }

  /* The process with rank 0 initializes the variables. */
  if (rank == 0) {
    double time1 = MPI_Wtime();
    image = stbi_load(argv[1], &width, &height, &bpp, 0);

    scale_x = 2;
    scale_y = 2;

    new_width = width / scale_x;
    new_height = height / scale_y;

    printf("Scale factor for x: %0.2f for y: %0.2f\n", scale_x, scale_y);
    printf("New size: %dx%d pixels\n", new_width, new_height);
  }

  /* Broadcast the variables to other processes. */
  MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&bpp, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&new_width, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&new_height, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&scale_x, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&scale_y, 1, MPI_INT, 0, MPI_COMM_WORLD);

  /* Each process initializes its own section variables. */
  int section_height = height / size;
  int section_y_start = rank * section_height;
  int section_y_end = section_y_start + section_height;

  uint8_t *section = (uint8_t *)malloc(width * section_height * bpp);

  /* Scatter the image, which is a matrix of unsigned 8-bit integers. */
  MPI_Scatter(image, width * section_height * bpp, MPI_UINT8_T, section,
              width * section_height * bpp, MPI_UINT8_T, 0, MPI_COMM_WORLD);

  int new_section_width = (float)width / scale_x;
  int new_section_height = (float)(section_y_end - section_y_start) / scale_y;

  /* Create an empty matrix to store the resized image section. */
  Pixel *scaled_section =
      (Pixel *)malloc(new_section_width * new_section_height * sizeof(Pixel));

  bilinear_scaling(section, scaled_section, bpp, width, section_height,
                   new_section_width, new_section_height, scale_x, scale_y,
                   rank);

  /* Create an empty matrix to store the resized image. */
  if (rank == 0)
    scaled_image = (Pixel *)malloc(new_width * new_height * sizeof(Pixel));

  /* Gather the scattered images. */
  MPI_Gather(scaled_section,
             new_section_width * new_section_height * sizeof(Pixel), MPI_BYTE,
             scaled_image,
             new_section_width * new_section_height * sizeof(Pixel), MPI_BYTE,
             0, MPI_COMM_WORLD);

  if (rank == 0) {
    stbi_write_jpg(argv[2], new_width, new_height, 3, scaled_image, 100);

    double time2 = MPI_Wtime();
    printf("Elapsed time: %lf\n", time2 - time1);

    stbi_image_free(image);
    free(scaled_image);
  }

  free(section);
  free(scaled_section);

  MPI_Finalize();
  return 0;
}
