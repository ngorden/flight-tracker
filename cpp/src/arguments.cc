#include "tracker.hh"

#include "arguments.hh"
#include <getopt.h>

namespace ft {
arguments::arguments(std::string_view start_location,
                     std::string_view end_location, long start_time,
                     long end_time)
    : m_start_location{start_location}, m_end_location{end_location},
      m_start_time{start_time}, m_end_time{end_time} {}

arguments parse_args(int argc, char **argv) {
  option long_opts[4] = {
      {"start-time", optional_argument, NULL, 's'},
      {"end-time", optional_argument, NULL, 'e'},
      {"start-location", optional_argument, NULL, 'f'},
      {"end-location", optional_argument, NULL, 't'},
  };

  char *start_location, *end_location;
  long start_time, end_time;

  while (true) {
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
      start_location = optarg;
      break;
    case 't':
      end_location = optarg;
      break;
    case '?':
      fprintf(stderr, "option '%c' not recognized.\n", optopt);
      break;
    }
  }

  return arguments(start_location, end_location, start_time, end_time);
}
} // namespace ft
