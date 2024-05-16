#pragma once

#include <cstdint>
#include <string>

uint32_t time_to_int(const std::string &time);
std::string time_to_str(const uint32_t time);
bool is_earlier(const std::string &time_original,
                const std::string &time_tested);
bool is_later(const std::string &time_original, const std::string &time_tested);
