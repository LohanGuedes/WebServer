#include "Client.hpp"
#include "AHttpRequest.hpp"
#include "Logger.hpp"
#include "RegularRequest.hpp"
#include "RunTime.hpp"
#include <cstddef>
#include <cstdlib>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

Client::Client(int const fd)
    : APollable(this->getEpollEventStruct()), request(NULL), response(NULL),
      pollout_ready(false) {
    this->_fd = fd;
}

Client::~Client(void) {
    if (this->request != NULL) {
        delete this->request;
    }
    if (this->_fd != -1) {
        close(this->_fd);
    }
    return;
}

void Client::handlePoll(epoll_event_bitflag const bitflag) {
    AHttpRequest *req;
    RunTime      *rt;

    if ((bitflag & EPOLLRDHUP) || (bitflag & EPOLLERR) ||
        (bitflag & EPOLLHUP)) {
        Logger::log(LOG_ERROR, "Hangup Occurred, cleaning request");
        // delete the request from the client. -> client
        // remove the request from the request requestPool -> rt
        // remove the client from the epoll instance -> rt
        rt = RunTime::getInstance();
        (void)rt->deleteClient(this);
        return;
    }
    if (bitflag & EPOLLIN) {
        // TODO: existe a possibilidade do usuário mandar exatamente o tamanho
        // do header na request, então esvaziaria o buffer interno e quando
        // chegasse o resto da request ele avisaria como um novo POLLIN.
        // Dessa forma ele criaria duas request (e vazaria a primeira).
        // Para evitar esse caso, a gente precisa checar se já existe uma
        // request pra esse client, caso exista, ligar uma flag de leitura
        // disponível na request, pra essa leitura ser processada pelo método de
        // handle da request (que o mesmo vai checar essa flag).
        Logger::log(LOG_WARNING, "Mocked Pollin on Client");
        req = this->parseHeader();
        if (!req) {
            ; // send bad request response
        }
        // add request to the client
        this->request = req;
        // push request to be processed by runTime
        rt = RunTime::getInstance();
        rt->requestPool.push_back(req);
        return;
    }
    if (bitflag & EPOLLOUT) {
        if (this->notifyResponseReady(NULL) == 0) {
            // TODO: Fazer o cleanup do senrResponse aqui?
            this->sendResponse();
        }
        return;
    }
    return;
}

// TODO: Implement this header parsing. It's mocked atm
AHttpRequest *Client::parseHeader(void) {
    srand(time(NULL));
    const int     numb = (rand() + 1) % 100;
    AHttpRequest *req;

    Logger::log(LOG_WARNING, "Mocked parseHeader on Client");
    switch (numb) {
    case 1 ... 33:
        req = new (std::nothrow) RegularRequest(this);
        break;
    case 34 ... 66:
        req = new (std::nothrow) MultipartRequest(this);
        break;
    case 67 ... 100:
        req = new (std::nothrow) CgiRequest(this);
        break;
    }
    return (req);
}

int Client::sendResponse(void) {
    // TODO implement this method
    return (0);
}

int Client::notifyResponseReady(HttpResponse *res) {
    (void)res;

    this->pollout_ready = true;
    if (this->response == NULL) {
        return (1);
    }
    this->sendResponse();
    // TODO: cleanup?
    RunTime *const rt = RunTime::getInstance();
    if (!rt) {
        // TODO: Implement analogous to panic!
    }
    rt->deleteClient(this);
    return (0);
}

struct epoll_event Client::getEpollEventStruct(void) const throw() {
    return ((const epoll_event){
        .events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET,
        .data = {.ptr = (void *)this},
    });
}
