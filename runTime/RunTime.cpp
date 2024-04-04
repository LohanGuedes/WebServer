#include "RunTime.hpp"

// Static variables have to be initialized like this
RunTime *RunTime::_instance = NULL;

RunTime::RunTime(void) {}

RunTime *RunTime::getInstance(void) {
  if (!RunTime::_instance) {
    RunTime::_instance = new RunTime();
  }
  return (RunTime::_instance);
}

void RunTime::deleteInstance(void) {
  // Implemente o cleanup das coisas no destrutor pra
  // gente usar RAII igual gente.
  if (!RunTime::_instance) {
    return;
  }
  delete RunTime::getInstance();
}

RunTime::~RunTime(void) {}

#if 0
// toda request que chegar vai ser um novo client, a não ser que seja uma
// request em chunk. Essa solução abaixo vai permitir que eu leia em partes
// requests com body muito grande e também requests em chunk.
// this is going to be the event loop for a request reading from the socket.
void read_from_sock() {
  static int status; // ou isso ou uma variável dentro de cada Request object
  int header_size;
  int body_status;

  switch (status) {
  case (0):
    header_size = read_header();
    if (header_size > HEADER_HARD_CEILING) {
      return (ERRO);
    }
    status++;
  case (1):
    body_status = processar_body();
    switch (body_status) {
    case (ERROR):
      return (ERRO);
      break;
    case (FINISHED):
      status++;
      break;
    case (STILL_TO_READ):
      break;
    }
  }
}
#endif
