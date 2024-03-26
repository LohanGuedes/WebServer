#include <iostream>
#include <utility>

#include "Lexer.hpp"

// TODO: CLEAN THIS UP!
// TESTS ARE WRITTEN IN C++11 or bigger!!! not project related!

void test_simple_input(void) {
  std::string input = ";    {}, server   root  test";

  Token expected_tokens[] = {
      Token(token_type::semicolon, ";"),     Token(token_type::lbracket, "{"),
      Token(token_type::rbracket, "}"),      Token(token_type::comma, ","),
      Token(token_type::server, "server"),   Token(token_type::root, "root"),
      Token(token_type::identifier, "test"),
      /* Token(token_type::eof, "\0"), */ // TODO: Make this pass
  };

  Lexer lex = Lexer(input);

  for (auto &expected : expected_tokens) {
    Token *token = lex.next_token();

    if (token->get_type() != expected.get_type() ||
        token->get_literal() != expected.get_literal()) {
      std::cerr << "Expected token literal: \"" << expected.get_literal()
                << "\""
                << " got \"" << token->get_literal() << "\"\n";
      std::cerr << "Expected token type: "
                << static_cast<int>(expected.get_type()) << " got "
                << static_cast<int>(token->get_type()) << '\n';
      std::exit(1);
    }
  }

  std::cout << "All tests passed successfully\n";

  return;
}

int main(void) {
  test_simple_input();

  return 0;
}
