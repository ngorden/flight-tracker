#ifndef __UTILS_H_INCLUDED__
#define __UTILS_H_INCLUDED__

typedef struct {
  long start_time;
  long end_time;
  char *start_location;
  char *end_location;
} args_t, *args_p;

args_t parse_args(int argc, char **argv);

#endif /* _UTILS_H_INCLUDED__ */
