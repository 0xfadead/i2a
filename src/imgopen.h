#ifndef _IMGOPEN_H_
#define _IMGOPEN_H_

unsigned char *load_image(const char *img_path, int *imgw, int *imgh, int *imgc,
                          int wanted_channels);

#endif
