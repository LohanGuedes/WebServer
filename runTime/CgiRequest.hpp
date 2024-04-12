#pragma once
#ifndef CGIREQUEST_HPP
#define CGIREQUEST_HPP
#include "AHttpRequest.hpp"

class CgiRequest : public AHttpRequest {
  public:
    CgiRequest(void);
    ~CgiRequest(void);
};

#endif
