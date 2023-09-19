#include "flight.hh"
#include "tracker.hh"

namespace ft {
flight_tracker::flight_tracker(arguments *arguments)
    : m_start_time{arguments->start_time()}, m_elapsed{0},
      m_total_trip{arguments->end_time() - arguments->start_time()} {}

double flight_tracker::percent_complete() {
  return static_cast<double>(m_elapsed) / static_cast<double>(m_total_trip);
}

void flight_tracker::tick() {
  time_t now = time(NULL);
  m_elapsed = now - m_start_time;
}
} // namespace ft
