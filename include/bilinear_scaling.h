#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned char red, green, blue;
} Pixel;

void bilinear_scaling(uint8_t *image, Pixel *pixels, int bpp, int width,
                          int height, int new_width, int new_height,
                          float scale_x, float scale_y, int rank);
