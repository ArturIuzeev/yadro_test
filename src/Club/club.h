#pragma once

#include <cstdint>
#include <deque>
#include <unordered_map>
#include <vector>

#include "client.h"
#include "club_util.h"
#include "table.h"

struct Club {
  Club(int table_count, const std::string &time_opened,
       const std::string &time_closed, int money);
  void client_arriving(const std::string &name, const std::string &time);
  void client_sitting(const std::string &name, const std::string &time,
                      const int table_id);
  void client_waiting(const std::string &name, const std::string &time);
  void client_leaving(const std::string &name, const std::string &time);

  std::vector<std::string> get_clients_in_club() const;
  int get_count_clients_in_club() const;
  int get_count_clients_in_queue() const;
  std::string get_first_client_in_queue() const;
  int get_table_id_by_client(std::string name) const;
  std::string get_client_by_table_id(int table_id) const;
  std::string get_time_opened() const;
  std::string get_time_closed() const;
  int get_tables_total() const;
  int get_tables_free() const;
  std::string get_all_tables_info() const;

  bool is_in_working_hours(std::string time);
  bool is_before_working_hours(std::string time);
  bool is_after_working_hours(std::string time);
  bool is_client_in_club(const std::string &name) const;

  bool is_client_in_queue(const std::string &name) const;
  bool is_client_arrived(const std::string &name) const;
  bool is_client_sitting(const std::string &name) const;

  bool is_table_occupied(int table_id) const;
  bool is_table_free(int table_id) const;

private:
  std::vector<Table> _tables;
  std::unordered_map<std::string, Client> _clients_in_club;
  std::deque<std::string> _clients_in_queue;
  uint32_t _time_opened;
  uint32_t _time_closed;
  int _tables_total;
  int _tables_free;
  int _money_per_hour;
};
