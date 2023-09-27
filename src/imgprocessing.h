#ifndef _IMG_PROCESSING_H_
#define _IMG_PROCESSING_H_

struct Image {
  unsigned char *img_data;
  int width;
  int height;
  int channels;
};

int convert_to_bw(const struct Image *src, struct Image *dst, int verbosity,
                  unsigned char char_len);

#endif
