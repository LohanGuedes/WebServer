#pragma once
#ifndef REGULARREQUEST_HPP
#define REGULARREQUEST_HPP
#include "AHttpRequest.hpp"

class RegularRequest : public AHttpRequest {
  public:
    RegularRequest(Client *const clientInstance);
    ~RegularRequest(void);
    virtual void processRequest(void) throw();
};

#endif // !REGULARREQUEST_HPP
