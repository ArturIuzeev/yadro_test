cmake_minimum_required(VERSION 3.28)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)

add_executable(yadro_test
        src/main.cpp
        src/ClubLogger/day_logger.cpp
        src/Club/table.cpp
        src/Club/club_util.cpp
        src/Club/club.cpp
        src/Club/client.cpp)
