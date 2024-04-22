#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "AHttpRequest.hpp"
#include "APollable.hpp"
#include "CgiRequest.hpp"
#include "IResponseNotifier.hpp"
#include "Logger.hpp"
#include "MultipartRequest.hpp"
#include "RegularRequest.hpp"
#include <cstdlib>
#include <new>
#include <sys/epoll.h>
#include <sys/socket.h>

class RunTime;
class HttpResponse;

class Client : public APollable, public IResponseNotifier {
  public:
    Client(int const fd);
    virtual ~Client(void);
    virtual void               handlePoll(epoll_event_bitflag const bitflag);
    AHttpRequest              *parseHeader(void);
    virtual int                notifyResponseReady(HttpResponse *res);
    virtual int                sendResponse(void);
    virtual struct epoll_event getEpollEventStruct(void) const throw();

    AHttpRequest const *request;
    HttpResponse       *response;
    bool                pollout_ready;
#if 0
	std::vector<ServerProps const *> serverProps;
#endif
};

// O cliente vai ter que parsear o header no handlePollin dele pra saber o
// tipo concreto do ponteiro abstrato 'request'.
// O método parseHeader atua como uma factory que vai parsear o header, criar a
// request correta e empurrar essa request para o loop de eventos.
// Por conta disso, o tamanho máximo do nosso header vai ter que ser o tamanho
// máximo do buffer de socket da máquina host.
#endif // !CLIENT_HPP
