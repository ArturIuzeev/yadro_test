#include "club_util.h"

uint32_t time_to_int(const std::string& time) {
    return static_cast<uint32_t>(std::stoi(time.substr(0, 2)) * 60 + std::stoi(time.substr(3, 2)));
}

std::string time_to_str(const uint32_t time) {
    std::string res;
    if (time / 60 < 10) {
        res += "0" + std::to_string(time / 60);
    } else {
        res += std::to_string(time / 60);
    }
    res += ":";
    if (time % 60 < 10) {
        res += "0" + std::to_string(time % 60);
    } else {
        res += std::to_string(time % 60);
    }
    return res;
}

bool is_earlier(const std::string& time_original, const std::string& time_tested) {
    return time_to_int(time_tested) < time_to_int(time_original);
}

bool is_later(const std::string& time_original, const std::string& time_tested) {
    return time_to_int(time_tested) > time_to_int(time_original);
}
