#include <iostream>

#include "ClubLogger/day_logger.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "error: invalid number of arguments, expected 2"; 
        return 1;
    }

    DayLogger().generate_day_log(argv[1]);
    return 0;
}

