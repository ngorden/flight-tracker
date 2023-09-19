#pragma once

namespace ft {
class arguments {
public:
  arguments(std::string_view start_location, std::string_view end_location,
            long start_time, long end_time);

  inline const long &end_time() { return m_end_time; }
  inline const long &start_time() { return m_start_time; }

  inline std::string_view start_location() { return m_start_location; }
  inline std::string_view end_location() { return m_end_location; }

private:
  std::string m_start_location;
  std::string m_end_location;
  long m_start_time;
  long m_end_time;
};

arguments parse_args(int argc, char **argv);
} // namespace ft
