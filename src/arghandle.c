#include "arghandle.h"
#define SHOW_COLOR
#define SHOW_TIMESTAMP
#include <loglib/llib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef bool
#include <stdbool.h>
#endif

const char *av0 = NULL;

void help(void) {
  printf("Usage for %s: %s [Options] <input image> <scale percentage>\n", av0,
         av0);
  printf("\tinput image:      The input image.\n");
  printf("\tscale percentage: The scale of the output image in percent.\n");
  printf("\n\tOptions:\n");
  printf("\t\t-v --verbose: Set the verbosity:\n");
  printf("\t\t\t1 / hard           = Disables all logs except for the "
         "important ones.\n");
  printf("\t\t\t2 / norm (default) = Only disables debug logs.\n");
  printf("\t\t\t3 / all            = Doesn't disable any logs.\n");
  printf("\n\tExamples:\n");
  printf("\t\t%s input.png 50\n", av0);
  printf("\t\t%s -v all input.png 50\n", av0);

  return;
}

struct Arguments handle_arguments(int ac, const char **av) {
  /* Used for making the arghandling option argnostic. */
  int current_arg = 0;
  /* Used for signaling a change of verbosity */
  bool verbose_set = false;
  /* set a global variable in order to not have to input av[0] every time help()
   * is called. */
  av0 = av[current_arg];

  /* Shut gcc up. */
  ll_verbosity = LL_LOG_NORMAL;

  struct Arguments args = {0};
  args.retcode = 0;

  /* The minimum is `./main <input image> <percentage>` so three arguments */
  if (ac < 3) {
    log_fatal("Too few arguments: Expected a minimum of 3 arguments (including "
              "argv0) but got %i!",
              ac);

    help();

    args.retcode = 1;

    goto ret_args;
  }

  current_arg++;

  args.verbosity_level = LL_LOG_NORMAL;

  /* check for the `--verbose` option */
  if (!strcmp(av[current_arg], "-v") || !strcmp(av[current_arg], "--verbose")) {
    current_arg++;
    if (!strcmp(av[current_arg], "1") || !strcmp(av[current_arg], "hard"))
      args.verbosity_level = LL_LOG_HARD;
    else if (!strcmp(av[current_arg], "2") || !strcmp(av[current_arg], "norm"))
      args.verbosity_level = LL_LOG_NORMAL;
    else if (!strcmp(av[current_arg], "3") || !strcmp(av[current_arg], "all"))
      args.verbosity_level = LL_LOG_ALL;
    else {
      log_fatal("Unknown option `%s` for argument `%s`", av[current_arg],
                av[current_arg - 1]);
      help();

      args.retcode = 2;

      goto ret_args;
    }

    verbose_set = true;

    current_arg++;
  }

  /* if there is a --verbose option the minimum jumps up to `./main -v
   * <verbosity option> <input image> <percentage>` so five arguments */
  if (verbose_set && ac < 5) {
    log_fatal("Expected 5 arguments (including argv0), because of the extra "
              "verbose option, but found %i!",
              ac);

    help();

    args.retcode = 3;
    goto ret_args;
  } else if (!verbose_set && ac > 3) {
    log_fatal("Expected 3 arguments (including argv0), but found %i!", ac);
    log_hard_info("Note: If you used the `--verbose` option at the end of the "
                  "arguments, please put at at the front.");

    help();

    args.retcode = 3;
    goto ret_args;
  }

  args.in_img_path = av[current_arg];
  current_arg++;

  args.in_percentage_str = av[current_arg];
  current_arg++;

  char *endptr = NULL;

  args.in_percentage = strtod(args.in_percentage_str, &endptr);

  if (args.in_percentage_str + strlen(args.in_percentage_str) > endptr) {
    log_fatal("Failed to parse percentage at char %lu!",
              endptr - args.in_percentage_str);

    args.retcode = 4;
    goto ret_args;
  }

  if (args.in_percentage <= 0) {
    log_fatal("Percentage is too small: Expected >= 0 but got %f",
              args.in_percentage);

    args.retcode = 5;
    goto ret_args;
  }

ret_args:
  return args;
}
