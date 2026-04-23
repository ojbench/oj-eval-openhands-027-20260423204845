#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

class LetStatement : public Statement {
 public:
  LetStatement(std::string source, std::string varName, Expression* expr);
  ~LetStatement() override;
  void execute(VarState& state, Program& program) const override;

 private:
  std::string varName_;
  Expression* expr_;
};

class PrintStatement : public Statement {
 public:
  PrintStatement(std::string source, Expression* expr);
  ~PrintStatement() override;
  void execute(VarState& state, Program& program) const override;

 private:
  Expression* expr_;
};

class InputStatement : public Statement {
 public:
  InputStatement(std::string source, std::string varName);
  ~InputStatement() override = default;
  void execute(VarState& state, Program& program) const override;

 private:
  std::string varName_;
};

class GotoStatement : public Statement {
 public:
  GotoStatement(std::string source, int targetLine);
  ~GotoStatement() override = default;
  void execute(VarState& state, Program& program) const override;

 private:
  int targetLine_;
};

class IfStatement : public Statement {
 public:
  IfStatement(std::string source, Expression* leftExpr, char op, Expression* rightExpr, int targetLine);
  ~IfStatement() override;
  void execute(VarState& state, Program& program) const override;

 private:
  Expression* leftExpr_;
  char op_;
  Expression* rightExpr_;
  int targetLine_;
};

class RemStatement : public Statement {
 public:
  RemStatement(std::string source, std::string comment);
  ~RemStatement() override = default;
  void execute(VarState& state, Program& program) const override;

 private:
  std::string comment_;
};

class EndStatement : public Statement {
 public:
  EndStatement(std::string source);
  ~EndStatement() override = default;
  void execute(VarState& state, Program& program) const override;
};
