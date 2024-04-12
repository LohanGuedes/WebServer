#pragma once
#ifndef REGULARREQUEST_HPP
#define REGULARREQUEST_HPP
#include "AHttpRequest.hpp"

class RegularRequest : public AHttpRequest {
  public:
    RegularRequest(void);
    ~RegularRequest(void);
};

#endif
