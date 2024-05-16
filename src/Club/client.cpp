#include "client.h"

#include <utility>

Client::Client(std::string name)
    : _name{std::move(name)}, _table_id{0}, _status{ClientStatus::ARRIVED} {}

void Client::sitting(int table_id) {
  _status = ClientStatus::SITTING;
  _table_id = table_id;
}

void Client::waiting() { _status = ClientStatus::WAITING; }

int Client::get_table_id() const { return _table_id; }

ClientStatus Client::check_status() const { return _status; }