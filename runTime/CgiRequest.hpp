#ifndef CGIREQUEST_HPP
#define CGIREQUEST_HPP

class CgiRequest : public AHttpRequest {
  public:
    CgiRequest(void);
    CgiRequest(CgiRequest const &cpy);
    ~CgiRequest(void);
    CgiRequest &operator=(CgiRequest const &rhs);
};

#endif
