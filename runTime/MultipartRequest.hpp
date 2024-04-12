#ifndef MULTIPARTREQUEST_HPP
#define MULTIPARTREQUEST_HPP
#include "AHttpRequest.hpp"

class MultipartRequest : public AHttpRequest {
  public:
    MultipartRequest(void);
    ~MultipartRequest(void);
};

#endif
