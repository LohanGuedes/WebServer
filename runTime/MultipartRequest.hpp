#ifndef MULTIPARTREQUEST_HPP
#define MULTIPARTREQUEST_HPP
#include "AHttpRequest.hpp"

class MultipartRequest : public AHttpRequest {
  public:
    MultipartRequest(Client const *const clientInstance);
    ~MultipartRequest(void);
};

#endif // !MULTIPARTREQUEST_HPP
