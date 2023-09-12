#include "tracker.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define BUFLEN (1 << 12)

#define PLANE_TAKEOFF ""
#define PLANE_LANDING ""
#define PLANE_FLYING ""

struct winsize winsz;
char *output_str;
char start_line[128] = "";
char start_time_str[128] = "";
char end_line[128] = "";
char end_time_str[128] = "";

long total_trip;
time_t start_time;
time_t end_time;

void main_loop() {
  char plane_str[16] = "";

#ifdef DEBUG
  puts("main loop");
#endif

  while (1) {
    time_t now = time(NULL);
    if (now >= end_time) {
#ifdef DEBUG
      printf("end: %ld\nnow: %ld\n", end_time, now);
#endif
      break;
    }

    long elapsed = now - start_time;
    double percent_complete = (double)elapsed / (double)total_trip;
    int plane_idx = (int)floor(
        (strlen(output_str) - strlen(end_line) - strlen(start_line)) *
        percent_complete);

    char *plane = percent_complete <= 0.15   ? PLANE_TAKEOFF
                  : percent_complete <= 0.85 ? PLANE_FLYING
                                             : PLANE_LANDING;

    snprintf(plane_str, sizeof plane_str, "%s (%.2f%%)", plane,
             100. * percent_complete);

    char final_string[BUFLEN];
    memset(final_string, '\0', BUFLEN);
    memcpy(final_string, output_str, strlen(output_str));
    memcpy(final_string + strlen(start_line) + plane_idx, plane_str,
           strlen(plane_str));

    printf("\033[2K%s\r", final_string);
    fflush(stdout);
    usleep(100000);
  }

  free(output_str);
}

void setup(args_p args_ptr) {
  args_t args = *args_ptr;
  output_str = (char *)malloc(BUFLEN * sizeof(char));
  strftime(start_time_str, sizeof start_time_str, "(%H:%M) %%s ",
           localtime(&args.start_time));
  snprintf(start_line, sizeof start_line, start_time_str, args.start_location);

  strftime(end_time_str, sizeof end_time_str, " %%s (%H:%M)",
           localtime(&args.end_time));
  snprintf(end_line, sizeof end_line, end_time_str, args.end_location);

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz);

  memset(output_str, '\0', BUFLEN);
  memset(output_str, '.', winsz.ws_col);

  memcpy(output_str, start_line, strlen(start_line));
  memcpy(output_str + strlen(output_str) - strlen(end_line), end_line,
         strlen(end_line));

  total_trip = args.end_time - args.start_time;
  start_time = args.start_time;
  end_time = args.end_time;

#ifdef DEBUG
  puts("Setup");
  printf("end: %ld\n", args.end_time);
#endif
}
