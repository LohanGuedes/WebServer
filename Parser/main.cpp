#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../runTime/Logger.hpp"
#include "Parser.hpp"
#include "ast/Ast.hpp"
#include "ast/IAst.hpp"
#include "ast/ListenStatement.hpp"

void test_listen_statements(void) {
    std::string input = "\
    listen localhost:1337;\
    listen localhost1234 bananinha;\
    listen localhost:4321;\
    ";

    std::string expected[] = {
        std::string("localhost:1337"),
        std::string("localhost:1234"),
        std::string("localhost:4321"),
    };

    Lexer        *lex = new Lexer(input);
    Parser        parser = Parser(lex);
    ServerConfig *serverConf = parser.ParseConfig();

    parser.check_parser_errors();

    if (serverConf == NULL) {
        std::cout << "Parserconfig returned a NULL pointer. exiting."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    if (serverConf->statements_len() != 3) {
        std::stringstream s;
        s << "Wrong number of statements. Expected 3 got: "
          << serverConf->statements_len() << std::endl;
        std::cout << s.str();
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < serverConf->statements.size(); i++) {
        IStatement      *stmt = serverConf->statements[i];
        ListenStatement *listenStmt = dynamic_cast<ListenStatement *>(stmt);

        if (listenStmt == NULL) {
            std::cout << "var stmt its not of the type ListenStatement"
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        if (stmt->token_literal() != "listen") {
            std::cout << "Wrong type of literal." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (listenStmt->host_port.value != expected[i]) {
            std::stringstream s;

            s << "Value differ from name. got: " << listenStmt->host_port.value
              << " Expected: " << expected[i] << std::endl;

            std::cout << s.str();
            exit(EXIT_FAILURE);
        }

        if (listenStmt->host_port.token_literal() != expected[i]) {
            std::stringstream s;
            s << "token_literal differ from expected. got: "
              << listenStmt->host_port.token_literal()
              << " Expected: " << expected[i] << std::endl;

            std::cout << s.str();
            exit(EXIT_FAILURE);
        }
    }
    std::cout << "All tests passed with success" << std::endl;
}

void test_location_statements(void) {
    std::string input = "\
    location /test/ { \
        allow_methods GET SOMETHING SOMETHING ELSE\
        return https://www.youtube.com;\
    };\
    ";

    std::string expected[] = {
        std::string("localhost:1337"),
        std::string("localhost:1234"),
        std::string("localhost:4321"),
    };

    Lexer        *lex = new Lexer(input);
    Parser        parser = Parser(lex);
    ServerConfig *serverConf = parser.ParseConfig();

    parser.check_parser_errors();

    if (serverConf == NULL) {
        std::cout << "Parserconfig returned a NULL pointer. exiting."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    if (serverConf->statements_len() != 3) {
        std::stringstream s;
        s << "Wrong number of statements. Expected 3 got: "
          << serverConf->statements_len() << std::endl;
        std::cout << s.str();
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < serverConf->statements.size(); i++) {
        IStatement      *stmt = serverConf->statements[i];
        ListenStatement *listenStmt = dynamic_cast<ListenStatement *>(stmt);

        if (listenStmt == NULL) {
            std::cout << "var stmt its not of the type ListenStatement"
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        if (stmt->token_literal() != "listen") {
            std::cout << "Wrong type of literal." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (listenStmt->host_port.value != expected[i]) {
            std::stringstream s;

            s << "Value differ from name. got: " << listenStmt->host_port.value
              << " Expected: " << expected[i] << std::endl;

            std::cout << s.str();
            exit(EXIT_FAILURE);
        }

        if (listenStmt->host_port.token_literal() != expected[i]) {
            std::stringstream s;
            s << "token_literal differ from expected. got: "
              << listenStmt->host_port.token_literal()
              << " Expected: " << expected[i] << std::endl;

            std::cout << s.str();
            exit(EXIT_FAILURE);
        }
    }
    std::cout << "All tests passed with success" << std::endl;
}

int main(void) {
    test_listen_statements();
    /* test_location_statements(); */
    return 0;
}
