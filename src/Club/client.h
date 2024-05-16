#pragma once

#include <string>

enum class ClientStatus { ARRIVED = 1, SITTING, WAITING };

struct Client {
  explicit Client(std::string name);
  void sitting(int table_id);
  void waiting();
  int get_table_id() const;
  ClientStatus check_status() const;

private:
  std::string _name;
  int _table_id;
  ClientStatus _status;
};
