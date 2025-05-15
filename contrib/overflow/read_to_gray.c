// SOURCED FROM:
// https://sourceforge.net/p/png-mng/mailman/png-mng-implement/thread/002b01cbb0e2%24ae636c80%240b2a4580%24@acm.org/

// gcc -std=c99 -g read_to_gray.c `libpng-config --cflags --ldflags --libs`
#include <stdio.h>
#include <png.h>

int
main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: %s FILE\n", argv[0]);
    return 1;
  }

  FILE *fp;
  if ((fp = fopen(argv[1], "rb")) == NULL)
    return 1;

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                   NULL, NULL, NULL);
  if (png_ptr == NULL)
    return 1;

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return 1;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 1;
  }

  png_init_io(png_ptr, fp);
  png_read_info(png_ptr, info_ptr);

  png_uint_32 width, height;
  int bit_depth, color_type, interlace_type;
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
               &interlace_type, NULL, NULL);

  png_set_palette_to_rgb(png_ptr);
  png_set_rgb_to_gray(png_ptr, 1, -1.0, -1.0);

  int number_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  png_bytep rowp = png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
  for (int pass = 0; pass < number_passes; pass++)
    for (int y = 0; y < height; y++)
      png_read_row(png_ptr, rowp, NULL);

  png_read_end(png_ptr, info_ptr);

  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(fp);

  return 0;
}