#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "../runTime/Logger.hpp"
#include "Parser.hpp"
#include "ast/Ast.hpp"
#include "ast/IAst.hpp"
#include "ast/ListenStatement.hpp"

void test_listen_statements(void) {
    std::string input = "\
    listen localhost:1337;\
    listen localhost:1234;\
    listen localhost:4321;\
    ";

    std::string expected[] = {
        std::string("localhost:1337"),
        std::string("localhost:1234"),
        std::string("localhost:4321"),
    };

    Lexer         lex = Lexer(input);
    Parser        parser = Parser(lex);
    ServerConfig *serverConf = parser.ParseConfig();
    if (serverConf == NULL) {
        Logger::log(LOG_ERROR,
                    "Parserconfig returned a NULL pointer. exiting.");
        exit(EXIT_FAILURE);
    }
    if (serverConf->statements_len() != 3) {
        std::stringstream s;
        s << "Wrong number of statements. Expected 3 got: "
          << serverConf->statements_len() << std::endl;
        Logger::log(LOG_ERROR, s.str());
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < serverConf->statements.size(); i++) {
        auto             stmt = serverConf->statements[i];
        ListenStatement *listenStmt = dynamic_cast<ListenStatement *>(stmt);

        if (listenStmt == NULL) {
            Logger::log(LOG_ERROR,
                        "var stmt its not of the type ListenStatement");
            exit(EXIT_FAILURE);
        }

        if (stmt->token_literal() != "listen") {
            Logger::log(LOG_ERROR, "Wrong type of literal.");
            exit(EXIT_FAILURE);
        }

        if (listenStmt->host_port->value != expected[i]) {
            std::stringstream s;

            s << "Value differ from name. got: " << listenStmt->host_port->value
              << " Expected: " << expected[i] << std::endl;

            Logger::log(LOG_ERROR, s.str());
            exit(EXIT_FAILURE);
        }

        if (listenStmt->host_port->token_literal() != expected[i]) {
            std::stringstream s;
            s << "token_literal differ from expected. got: "
              << listenStmt->host_port->token_literal()
              << " Expected: " << expected[i] << std::endl;

            Logger::log(LOG_ERROR, s.str());
            exit(EXIT_FAILURE);
        }
    }
}
