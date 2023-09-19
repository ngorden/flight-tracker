cmake_minimum_required(VERSION 3.27)

project(tracker)

add_executable(tracker
    src/arguments.cc src/arguments.hh
    src/flight.cc src/flight.hh
    src/tracker.cc src/tracker.hh)

target_precompile_headers(tracker PUBLIC src/tracker.hh)
