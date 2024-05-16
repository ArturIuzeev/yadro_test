#include <sstream>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "day_logger.h"

namespace fs = std::filesystem;

void DayLogger::generate_day_log(const std::string& filename) {
    std::string buffer;
    fs::path path_to_project_root {fs::current_path().parent_path()};
    std::ifstream fin(path_to_project_root / "tmp" / filename);
    if (!fin) { 
        std::cerr << "error: input file can't be opened" << std::endl;
    }

    std::ofstream fout(path_to_project_root / "log" / ("log_" + filename));
    if (!fout) { 
        std::cerr << "error: output file can't be opened" << std::endl;
    }

    if (!is_valid_file(fin)) {
        return;
    }

    std::vector<std::string> club_init_params;
    for (int i = 0; i < 3; ++i) {
        std::getline(fin, buffer);
        club_init_params.push_back(buffer);
    }

    std::vector<std::string> working_hours {split_str(club_init_params[1])};
    Club club(std::stoi(club_init_params[0]),
            working_hours[0],
            working_hours[1],
            std::stoi(club_init_params[2]));

    fout << working_hours[0] << std::endl;
    
    bool flag_end_of_day = false;
    std::vector<std::string> split_buffer;
    while (std::getline(fin, buffer)) {
        split_buffer = split_str(buffer);

        if (club.is_after_working_hours(split_buffer[0])) {
            flag_end_of_day = true;
            handle_end_of_working_day(fout, club);
        }

        fout << buffer << "\n";
        handle_input_event(fout, split_buffer, club);
    }

    if (!flag_end_of_day) {
        handle_end_of_working_day(fout, club);
    }
    
    fout << working_hours[1] << std::endl;

    fout << club.get_all_tables_info();

    fin.close();
    fout.close();
}

void DayLogger::handle_input_event(std::ostream& os, const std::vector<std::string>& event_line, Club& club) {
    std::string response;
    switch (std::stoi(event_line[1])) {
        case 1:
            response = handle_arriving(event_line, club);
            break;
        case 2:
            response = handle_sitting(event_line, club);
            break;
        case 3:
            response = handle_waiting(event_line, club);
            break;
        case 4:
            response = handle_leaving(event_line, club);
            break;
    }
    if (!response.empty()) {
        os << response;
    }
}

void DayLogger::handle_end_of_working_day(std::ostream& os, Club& club) {

    std::vector<std::string> clients_in_club {club.get_clients_in_club()};
    std::set<std::string> clients_in_club_alphabetic;
    for (const std::string& s : clients_in_club) {
        clients_in_club_alphabetic.insert(s);
    }

    for (const std::string& s : clients_in_club_alphabetic) {
        os << handle_leaving_end_of_day(s, club);
    }
}

std::string DayLogger::handle_arriving(const std::vector<std::string>& event_line, Club& club) {
    std::string response;

    if (club.is_client_in_club(event_line[2])) { 
        response = generate_response(event_line[0], EventIds::ERROR, "YouShallNotPass");
    } else if (!club.is_in_working_hours(event_line[0])) {
        response = generate_response(event_line[0], EventIds::ERROR, "NotOpenYet");
    }
    club.client_arriving(event_line[2], event_line[0]);

    return response;
}

std::string DayLogger::handle_sitting(const std::vector<std::string>& event_line, Club& club) {
    std::string response;

    if (!club.is_client_in_club(event_line[2])) {
        response = generate_response(event_line[0], EventIds::ERROR, "ClientUnknown");
    } else if (club.is_table_occupied(std::stoi(event_line[3]))) { 
        response = generate_response(event_line[0], EventIds::ERROR, "PlaceIsBusy");
    }
    club.client_sitting(event_line[2], event_line[0], std::stoi(event_line[3]));

    return response;
}

std::string DayLogger::handle_waiting(const std::vector<std::string>& event_line, Club& club) { 
    std::string response;

    if (club.get_tables_free() > 0) {
        response = generate_response(event_line[0], EventIds::ERROR, "ICanWaitNoLonger!");
    } else if (club.get_count_clients_in_queue() > club.get_tables_total()) {
        response = generate_response(event_line[0], EventIds::LEAVING_END, event_line[2]);
    }
    club.client_waiting(event_line[2], event_line[0]);

    return response;
}

std::string DayLogger::handle_leaving(const std::vector<std::string>& event_line, Club& club) {
    std::string response;

    if (!club.is_client_in_club(event_line[2])) {
        response = generate_response(event_line[0], EventIds::ERROR, "ClientUnknown");
    } else {
        if (club.is_client_sitting(event_line[2]) && club.get_count_clients_in_queue() > 0 && club.is_in_working_hours(event_line[0])) {
            int table_id = club.get_table_id_by_client(event_line[2]);
            std::string next_client {club.get_first_client_in_queue()};
            response = generate_response(event_line[0], EventIds::SITTING_FROM_QUEUE, next_client + " " + std::to_string(table_id));
        }
    }
    club.client_leaving(event_line[2], event_line[0]);

    return response;
}

std::string DayLogger::handle_leaving_end_of_day(const std::string& client, Club& club) {
    std::string response = generate_response(club.get_time_closed(), EventIds::LEAVING_END, client);

    club.client_leaving(client, club.get_time_closed());

    return response;
}

std::string DayLogger::generate_response(const std::string& time, EventIds id, const std::string& mess) {
    return time + " " + std::to_string(static_cast<int>(id)) + " " + mess + "\n";
}

std::vector<std::string> DayLogger::split_str(const std::string& str) {
    std::vector<std::string> res;

    std::stringstream ss(str);
    std::string part;
    while (ss >> part) {
        res.push_back(part);
    }

    return res;
}

bool DayLogger::is_valid_file(std::istream& is) {
    std::string buffer;

    std::regex regex_tables("[0-9]+[ ]*");
    std::getline(is, buffer);
    if (!std::regex_match(buffer, regex_tables)) {
        std::cout << buffer << std::endl;
        return false;
    }

    std::regex regex_time("[0-9]{2}:[0-9]{2} [0-9]{2}:[0-9]{2}[ ]*");
    std::getline(is, buffer);
    if (!std::regex_match(buffer, regex_time)) {
        std::cout << buffer << std::endl;
        return false;
    }

    std::regex regex_price("[0-9]+[ ]*");
    std::getline(is, buffer);
    if (!std::regex_match(buffer, regex_price)) {
        std::cout << buffer << std::endl;
        return false;
    }

    std::regex regex_event("[0-9]{2}:[0-9]{2} [1-4] [a-z0-9_-]+[ ]?[0-9]*[ ]*");
    std::string prev_time("00:00");
    while (std::getline(is, buffer)) {
        std::string cur_time;
        std::stringstream ss(buffer);
        ss >> cur_time;

        if (!std::regex_match(buffer, regex_event) || is_earlier(prev_time, cur_time) || is_later("23:59", cur_time)) {
            std::cout << buffer << std::endl;
            return false;
        }
        prev_time = cur_time;
    }

    is.clear();
    is.seekg(0);

    return true;
}
