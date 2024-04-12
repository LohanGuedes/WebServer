#pragma once
#ifndef REGULARREQUEST_HPP
#define REGULARREQUEST_HPP
#include "AHttpRequest.hpp"

class RegularRequest : public AHttpRequest {
  public:
    RegularRequest(Client const *const clientInstance);
    ~RegularRequest(void);
};

#endif // !REGULARREQUEST_HPP
