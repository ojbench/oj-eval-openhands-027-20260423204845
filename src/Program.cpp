#include "Program.hpp"
#include "Statement.hpp"
#include "utils/Error.hpp"

Program::Program() : programCounter_(0), programEnd_(false) {}

void Program::addStmt(int line, Statement* stmt) {
  recorder_.add(line, stmt);
}

void Program::removeStmt(int line) {
  recorder_.remove(line);
}

void Program::run() {
  if (recorder_.hasLine(0)) {
    throw BasicError("LINE NUMBER ERROR");
  }
  
  resetAfterRun();
  
  int currentLine = recorder_.nextLine(0);
  if (currentLine == -1) {
    return;
  }
  
  changePC(currentLine);
  
  while (!programEnd_) {
    const Statement* stmt = recorder_.get(getPC());
    if (!stmt) {
      break;
    }
    
    stmt->execute(vars_, *this);
    
    if (!programEnd_) {
      int nextLine = recorder_.nextLine(getPC());
      if (nextLine == -1) {
        break;
      }
      changePC(nextLine);
    }
  }
}

void Program::list() const {
  recorder_.printLines();
}

void Program::clear() {
  recorder_.clear();
  vars_.clear();
  resetAfterRun();
}

void Program::execute(Statement* stmt) {
  if (stmt) {
    stmt->execute(vars_, *this);
    delete stmt;
  }
}

int Program::getPC() const noexcept {
  return programCounter_;
}

void Program::changePC(int line) {
  if (line < 0) {
    throw BasicError("LINE NUMBER ERROR");
  }
  programCounter_ = line;
}

void Program::programEnd() {
  programEnd_ = true;
}

void Program::resetAfterRun() noexcept {
  programCounter_ = 0;
  programEnd_ = false;
  vars_.clear();
}