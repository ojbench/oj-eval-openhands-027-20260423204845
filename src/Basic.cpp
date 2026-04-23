#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      TokenStream tokens = lexer.tokenize(line);
      
      if (tokens.empty()) {
        continue;
      }
      
      const Token* firstToken = tokens.peek();
      if (!firstToken) {
        continue;
      }
      
      // Check for interpreter commands
      if (firstToken->type == TokenType::RUN) {
        program.run();
      } else if (firstToken->type == TokenType::LIST) {
        program.list();
      } else if (firstToken->type == TokenType::CLEAR) {
        program.clear();
      } else if (firstToken->type == TokenType::QUIT) {
        break;
      } else if (firstToken->type == TokenType::HELP) {
        std::cout << "BASIC Interpreter Commands:\n";
        std::cout << "RUN - Run the program\n";
        std::cout << "LIST - List all program lines\n";
        std::cout << "CLEAR - Clear all program lines\n";
        std::cout << "QUIT - Exit the interpreter\n";
        std::cout << "HELP - Show this help message\n";
        std::cout << "LET <var> = <expr> - Assign value to variable\n";
        std::cout << "PRINT <expr> - Print expression value\n";
        std::cout << "INPUT <var> - Input value for variable\n";
        std::cout << "<line> <stmt> - Add program line\n";
        std::cout << "<line> - Delete program line\n";
      } else {
        // Parse as program line or immediate statement
        ParsedLine parsed = parser.parseLine(tokens, line);
        
        if (parsed.getLine().has_value()) {
          int lineNumber = parsed.getLine().value();
          Statement* stmt = parsed.fetchStatement();
          
          if (stmt) {
            program.addStmt(lineNumber, stmt);
          } else {
            // Just a line number, delete the line
            program.removeStmt(lineNumber);
          }
        } else {
          // Immediate statement
          Statement* stmt = parsed.fetchStatement();
          program.execute(stmt);
        }
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}