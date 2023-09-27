#ifndef _ARGHANDLE_H_
#define _ARGHANDLE_H_

extern const char *av0;

struct Arguments {
  int retcode;
  int verbosity_level;
  const char *in_img_path;
  const char *in_percentage_str;
  double in_percentage;
};

struct Arguments handle_arguments(int ac, const char **av);

#endif
