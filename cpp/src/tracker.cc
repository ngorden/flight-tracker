#include "tracker.hh"
#include <iomanip>
#include <sys/ioctl.h>

#include "flight.hh"

#define PLANE_FLYING ""
#define PLANE_LANDING ""
#define PLANE_TAKEOFF ""

int main(int argc, char **argv) {
  auto args = ft::parse_args(argc, argv);
  winsize termsz;

  auto start_time_tm = std::localtime(&args.start_time());
  auto end_time_tm = std::localtime(&args.end_time());

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &termsz);
  std::stringstream start_builder, end_builder;
  start_builder << "(" << std::put_time(start_time_tm, "%R") << ") "
                << args.start_location() << " ";
  end_builder << " (" << std::put_time(end_time_tm, "%R") << ") "
              << args.end_location();

  std::string starting_str = std::string(termsz.ws_col, '.');
  starting_str = starting_str.replace(0, start_builder.str().length(),
                                      start_builder.str());
  starting_str =
      starting_str.replace(termsz.ws_col - end_builder.str().length(),
                           end_builder.str().length(), end_builder.str());

  for (auto ft = ft::flight_tracker(&args); !ft.is_trip_complete(); ft.tick()) {
    auto percent = ft.percent_complete();
    int precision = percent >= 10 ? 5 : 4;
    std::stringstream progress_str;
    progress_str.precision(precision);
    progress_str << PLANE_FLYING << " (" << percent * 100. << "%)";
    int idx = start_builder.str().length() +
              static_cast<uint64_t>((starting_str.length() -
                                     progress_str.str().length() -
                                     end_builder.str().length()) *
                                    ft.percent_complete());
    std::string output_str = starting_str;
    output_str = output_str.replace(idx, progress_str.str().length(),
                                    progress_str.str());

    std::cout << "\033[2K" << output_str << "\r";
    usleep(10000);
    std::fflush(stdout);
  }

  std::cout << "\033[2KFlight completed!" << std::endl;
  return EXIT_SUCCESS;
}
