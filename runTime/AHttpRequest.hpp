#ifndef AHTTPREQUEST_HPP
#define AHTTPREQUEST_HPP

#include "IResponseNotifier.hpp"

class Client; // forward declaration

class AHttpRequest : public IResponseNotifier {
  public:
    AHttpRequest(Client *const clientInstance);
    virtual ~AHttpRequest(void);
    virtual void processRequest(void) throw() = 0;
    virtual int  notifyResponseReady(HttpResponse *res);

  protected:
    Client *const _clientInstance;
};

#endif // !AHTTPREQUEST_HPP
