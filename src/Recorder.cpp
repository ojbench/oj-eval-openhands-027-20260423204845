#include "Recorder.hpp"
#include "Statement.hpp"
#include <iostream>

Recorder::~Recorder() {
  for (auto& pair : statements_) {
    delete pair.second;
  }
}

void Recorder::add(int line, Statement* stmt) {
  auto it = statements_.find(line);
  if (it != statements_.end()) {
    delete it->second;
  }
  statements_[line] = stmt;
}

void Recorder::remove(int line) {
  auto it = statements_.find(line);
  if (it != statements_.end()) {
    delete it->second;
    statements_.erase(it);
  }
}

const Statement* Recorder::get(int line) const noexcept {
  auto it = statements_.find(line);
  if (it != statements_.end()) {
    return it->second;
  }
  return nullptr;
}

bool Recorder::hasLine(int line) const noexcept {
  return statements_.find(line) != statements_.end();
}

void Recorder::clear() noexcept {
  for (auto& pair : statements_) {
    delete pair.second;
  }
  statements_.clear();
}

void Recorder::printLines() const {
  for (const auto& pair : statements_) {
    std::cout << pair.first << " " << pair.second->text() << "\n";
  }
}

int Recorder::nextLine(int line) const noexcept {
  auto it = statements_.upper_bound(line);
  if (it != statements_.end()) {
    return it->first;
  }
  return -1;
}