#pragma once
#ifndef CGIREQUEST_HPP
#define CGIREQUEST_HPP
#include "AHttpRequest.hpp"

class CgiRequest : public AHttpRequest {
  public:
    CgiRequest(Client const *const clientInstance);
    ~CgiRequest(void);
    virtual void processRequest(void) throw();
};

#endif // !CGIREQUEST_HPP
