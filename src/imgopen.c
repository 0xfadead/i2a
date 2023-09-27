#define STB_IMAGE_IMPLEMENTATION
#include "imgopen.h"
#define SHOW_COLOR
#define SHOW_TIMESTAMP
#include <llib.h>
#include <stb/stb_image.h>

unsigned char *load_image(const char *img_path, int *imgw, int *imgh, int *imgc,
                          int wanted_channels) {
  ll_verbosity = LL_LOG_NORMAL;
  unsigned char *opened_image = NULL;

  opened_image = stbi_load(img_path, imgw, imgh, imgc, wanted_channels);

  if (!opened_image) {
    log_fatal("Could not load image `%s`: %s", img_path, stbi_failure_reason());

    return NULL;
  }

  return opened_image;
}
