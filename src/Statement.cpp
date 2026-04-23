#include "Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

LetStatement::LetStatement(std::string source, std::string varName, Expression* expr)
    : Statement(std::move(source)), varName_(std::move(varName)), expr_(expr) {}

LetStatement::~LetStatement() {
  delete expr_;
}

void LetStatement::execute(VarState& state, Program& program) const {
  int value = expr_->evaluate(state);
  state.setValue(varName_, value);
}

PrintStatement::PrintStatement(std::string source, Expression* expr)
    : Statement(std::move(source)), expr_(expr) {}

PrintStatement::~PrintStatement() {
  delete expr_;
}

void PrintStatement::execute(VarState& state, Program& program) const {
  int value = expr_->evaluate(state);
  std::cout << value << "\n";
}

InputStatement::InputStatement(std::string source, std::string varName)
    : Statement(std::move(source)), varName_(std::move(varName)) {}

void InputStatement::execute(VarState& state, Program& program) const {
  std::cout << "? ";
  std::string input;
  std::getline(std::cin, input);
  
  try {
    size_t pos;
    int value = std::stoi(input, &pos);
    if (pos != input.length()) {
      throw BasicError("INVALID INPUT");
    }
    state.setValue(varName_, value);
  } catch (const std::out_of_range&) {
    throw BasicError("INVALID INPUT");
  } catch (const std::invalid_argument&) {
    throw BasicError("INVALID INPUT");
  }
}

GotoStatement::GotoStatement(std::string source, int targetLine)
    : Statement(std::move(source)), targetLine_(targetLine) {}

void GotoStatement::execute(VarState& state, Program& program) const {
  program.changePC(targetLine_);
}

IfStatement::IfStatement(std::string source, Expression* leftExpr, char op, Expression* rightExpr, int targetLine)
    : Statement(std::move(source)), leftExpr_(leftExpr), op_(op), rightExpr_(rightExpr), targetLine_(targetLine) {}

IfStatement::~IfStatement() {
  delete leftExpr_;
  delete rightExpr_;
}

void IfStatement::execute(VarState& state, Program& program) const {
  int leftValue = leftExpr_->evaluate(state);
  int rightValue = rightExpr_->evaluate(state);
  
  bool condition = false;
  switch (op_) {
    case '=':
      condition = (leftValue == rightValue);
      break;
    case '<':
      condition = (leftValue < rightValue);
      break;
    case '>':
      condition = (leftValue > rightValue);
      break;
    default:
      throw BasicError("UNSUPPORTED OPERATOR");
  }
  
  if (condition) {
    program.changePC(targetLine_);
  }
}

RemStatement::RemStatement(std::string source, std::string comment)
    : Statement(std::move(source)), comment_(std::move(comment)) {}

void RemStatement::execute(VarState& state, Program& program) const {
  // REM statements do nothing
}

EndStatement::EndStatement(std::string source)
    : Statement(std::move(source)) {}

void EndStatement::execute(VarState& state, Program& program) const {
  program.programEnd();
}
