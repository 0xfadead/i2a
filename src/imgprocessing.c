#include "imgprocessing.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define SHOW_COLOR
#define SHOW_TIMESTAMP
#include <llib.h>

int convert_to_bw(const struct Image *src, struct Image *dst, int verbosity,
                  unsigned char char_len) {
  ll_verbosity = verbosity;

  unsigned long errors = 0;

  dst->width = src->width;
  dst->height = src->height;
  dst->channels = 1;

  log_soft_progress("Allocating memory for new image...");
  dst->img_data =
      (unsigned char *)calloc(src->width * src->height, sizeof(unsigned char));
  if (!dst->img_data) {
    log_fatal("Couldn't allocate memory for new image: %s", strerror(errno));

    return 9;
  }

  for (unsigned long i = 0; i < (unsigned long)src->width * src->height; i++) {
    for (unsigned char c = 0; c < src->channels; c++) {
      dst->img_data[i] +=
          src->img_data[(i * src->channels) + c] / src->channels;
    }
    dst->img_data[i] /= (255.0 / char_len);

    if (dst->img_data[i] >= char_len) {
      log_soft_error("error at x=%04i y=%04i: %i >= %i", (int)(i % src->width),
                     (int)(i / src->width), dst->img_data[i], char_len);
      dst->img_data[i] = char_len - 1;
      errors++;
    }
  }

  log_info("Errors: %lu", errors);

  return 0;
}
