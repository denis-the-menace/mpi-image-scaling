#include "../include/bilinear_scaling.h"

void bilinear_scaling(uint8_t *image, Pixel *pixels, int bpp, int width,
                      int height, int new_width, int new_height, float scale_x,
                      float scale_y, int rank) {

  for (int y = 0; y < new_height; y++) {
    for (int x = 0; x < new_width; x++) {
      Pixel pixel = {255, 255, 255};

      /* Scale the pixel coordinates */
      float src_x = x * scale_x;
      float src_y = y * scale_y;

      /*  Get the four surrounding pixels: */
      /*  x1,y1 --------- x2,y1   */
      /*    |               |     */
      /*    |               |     */
      /*    |       P       |     */
      /*    |               |     */
      /*    |               |     */
      /*  x1,y2 --------- x2,y2   */
      int x1 = (int)floorf(src_x);
      int y1 = (int)floorf(src_y);
      int x2 = x1 + 1;
      int y2 = y1 + 1;

      /* Compute the weights for each pixel */
      float w1 = (x2 - src_x) * (y2 - src_y);
      float w2 = (src_x - x1) * (y2 - src_y);
      float w3 = (x2 - src_x) * (src_y - y1);
      float w4 = (src_x - x1) * (src_y - y1);

      /* Clamp the pixel coordinates to the image boundaries. */
      /* Since we are only downscaling the image, this part is not required. */
      /* x1 = (x1 < 0) ? 0 : ((x1 >= width) ? width - 1 : x1); */
      /* x2 = (x2 < 0) ? 0 : ((x2 >= width) ? width - 1 : x2); */
      /* y1 = (y1 < 0) ? 0 : ((y1 >= height) ? height - 1 : y1); */
      /* y2 = (y2 < 0) ? 0 : ((y2 >= height) ? height - 1 : y2); */

      /* Get the colors of the four surrounding pixels */
      int index1 = (y1 * width + x1) * bpp;
      int index2 = (y1 * width + x2) * bpp;
      int index3 = (y2 * width + x1) * bpp;
      int index4 = (y2 * width + x2) * bpp;

      Pixel pixel1 = {image[index1], image[index1 + 1], image[index1 + 2]};
      Pixel pixel2 = {image[index2], image[index2 + 1], image[index2 + 2]};
      Pixel pixel3 = {image[index3], image[index3 + 1], image[index3 + 2]};
      Pixel pixel4 = {image[index4], image[index4 + 1], image[index4 + 2]};

      /* Compute the weighted average of the colors */
      pixel.red = (unsigned char)(w1 * pixel1.red + w2 * pixel2.red +
                                  w3 * pixel3.red + w4 * pixel4.red);
      pixel.green = (unsigned char)(w1 * pixel1.green + w2 * pixel2.green +
                                    w3 * pixel3.green + w4 * pixel4.green);
      pixel.blue = (unsigned char)(w1 * pixel1.blue + w2 * pixel2.blue +
                                   w3 * pixel3.blue + w4 * pixel4.blue);

      /* Set the pixel color in the scaled image */
      pixels[y * new_width + x] = pixel;
    }
  }
}
