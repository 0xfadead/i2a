#define SHOW_COLOR
#define SHOW_TIMESTAMP
#include <llib.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arghandle.h"
#include "imgopen.h"
#include "imgprocessing.h"

char chars[] =
    " `.-':_,^=;><+!rc*/"
    "z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

int main(int ac, const char *av[]) {
  /* The dimensions and channels of the inputed image */
  int inw, inh, inc;

  unsigned char *in_file_img = NULL;
  unsigned char chars_len = (unsigned char)strlen(chars);

  /* handle arguments */
  struct Arguments args = handle_arguments(ac, av);
  ll_verbosity = args.verbosity_level;

  /* check if there was an error and return if there was one */
  if (args.retcode != 0)
    return args.retcode;

  log_soft_progress("Loading image with stbi_load(%s, &inw, &inh, &inc, 0)...",
                    args.in_img_path);

  /* load the image with custom function in order for not having to compile stb
   * all the time. */
  in_file_img = load_image(args.in_img_path, &inw, &inh, &inc, 0);
  if (!in_file_img)
    return 6;

  /* We make outw double as big, because characters are 1x2 */
  int outw = (int)(inw * 2 * args.in_percentage / 100);
  int outh = (int)(inh * args.in_percentage / 100);

  log_soft_info("%ix%ix%i * %.2f%% -> %ix%ix%i", inw, inh, inc,
                args.in_percentage, outw / 2, outh, 1);

  log_soft_progress("Resizing image...");
  struct Image resized_img = (struct Image){
      .width = outw,
      .height = outh,
      .channels = inc,
      .img_data =
          (unsigned char *)malloc(outw * outh * inc * sizeof(unsigned char)),
  };

  if (!resized_img.img_data) {
    log_fatal("Couldn't allocate memory for resized image: %s",
              strerror(errno));

    return 7;
  }

  int res = stbir_resize_uint8(in_file_img, inw, inh, 0, resized_img.img_data,
                               outw, outh, 0, inc);

  if (!res) {
    log_fatal("Couldn't resize input image!");

    return 8;
  }

  /* We don't need the old image anymore */
  log_soft_progress("Freeing input image...");
  free(in_file_img);

  /* Convert image to black and white */
  log_soft_progress("Converting image to black and white...");
  log_soft_info("Using %i characters.", chars_len);
  struct Image outimg = {0};
  res = convert_to_bw(&resized_img, &outimg, ll_verbosity, chars_len);
  if (res != 0)
    return res;

  log_soft_info("Freeing resized image...");
  free(resized_img.img_data);

  log_soft_progress("Printing image...");
  for (int y = 0; y < outh; y++) {
    for (int x = 0; x < outw; x++) {
      unsigned char cpix = chars[outimg.img_data[(y * outw) + x]];

      printf("%c", cpix);
    }
    puts("");
  }

  /* Cleanup */
  log_soft_progress("Freeing output image...");
  free(outimg.img_data);

  return EXIT_SUCCESS;
}
