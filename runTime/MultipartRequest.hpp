#ifndef MULTIPARTREQUEST_HPP
#define MULTIPARTREQUEST_HPP
#include "AHttpRequest.hpp"

class MultipartRequest : public AHttpRequest {
  public:
    MultipartRequest(Client const *const clientInstance);
    ~MultipartRequest(void);
    virtual void processRequest(void) throw();
};

#endif // !MULTIPARTREQUEST_HPP
