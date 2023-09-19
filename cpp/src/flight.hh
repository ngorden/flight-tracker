#pragma once

#ifndef __FLIGHT_TRACKER_INCLUDES_ARGUMENTS_HH__
#define __FLIGHT_TRACKER_INCLUDES_ARGUMENTS_HH__
#include "arguments.hh"
#endif

namespace ft {
class flight_tracker {
public:
  flight_tracker(arguments *arguments);

  inline bool is_trip_complete() { return m_elapsed >= m_total_trip; }
  double percent_complete();
  void tick();

private:
  long m_elapsed;
  long m_start_time;
  long m_total_trip;
};
} // namespace ft
