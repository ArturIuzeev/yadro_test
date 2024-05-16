#include "table.h"
#include "club_util.h"

Table::Table(int id) : _id{id},
    _cur_client{""},
    _time_cur_sitted{0},
    _time_total{0},
    _money{0},
    _status{TableStatus::FREE}
{}

void Table::client_sitting(const std::string& client, const std::string& time) {
    _cur_client = client;
    _time_cur_sitted = time_to_int(time);
    _status = TableStatus::OCCUPIED;
}

void Table::client_standing(const std::string& time, int money_per_hour) {
    _cur_client = "";
    int tmp_time = time_to_int(time) - _time_cur_sitted; 
    _time_total += tmp_time; 
    _money += money_per_hour * (tmp_time / 60);
    if (tmp_time % 60) {
        _money += money_per_hour;
    }
    _status = TableStatus::FREE;
}

std::string Table::current_info() const {
    return std::to_string(_id) + " " + std::to_string(_money) + " " + time_to_str(_time_total);
}
