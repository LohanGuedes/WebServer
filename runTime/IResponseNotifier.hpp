#ifndef IRESPONDER_HPP
#define IRESPONDER_HPP
#pragma once

class Client;
class HttpResponse;

class IResponseNotifier {
  public:
    IResponseNotifier(void) {}
    IResponseNotifier(IResponseNotifier const &cpy) { (void)cpy; }
    virtual ~IResponseNotifier(void) {}
    virtual IResponseNotifier &operator=(IResponseNotifier const &rhs) {
        (void)rhs;
        return *this;
    }
    virtual int notifyResponseReady(HttpResponse *res) = 0;
};

#endif
