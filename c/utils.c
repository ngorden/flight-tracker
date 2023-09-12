#include "utils.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

args_t parse_args(int argc, char **argv) {
  struct option long_opts[4] = {
      {"start-time", optional_argument, NULL, 's'},
      {"end-time", optional_argument, NULL, 'e'},
      {"start-location", optional_argument, NULL, 'f'},
      {"end-location", optional_argument, NULL, 't'},
  };

  char *start_loc = "", *end_loc = "";
  long start_time = 0, end_time = 0;

  while (1) {
    int c = getopt_long(argc, argv, "s:e:f:t:", long_opts, NULL);

    if (c == -1) {
      break;
    }

    switch (c) {
    case 's':
      start_time = atol(optarg);
      break;
    case 'e':
      end_time = atol(optarg);
      break;
    case 'f':
      start_loc = optarg;
      break;
    case 't':
      end_loc = optarg;
      break;
    case '?':
      printf("option '%c' not recognized.\n", optopt);
      break;
    }
  }

  return (args_t){
      .start_time = start_time,
      .end_time = end_time,
      .start_location = start_loc,
      .end_location = end_loc,
  };
}
