#include <stdio.h>
#include <stdlib.h>

#include "tracker.h"
#include "utils.h"

int main(int argc, char **argv) {
  args_t args = parse_args(argc, argv);

  setup(&args);
  main_loop();

  puts("\033[2KFlight completed!");
  return EXIT_SUCCESS;
}
