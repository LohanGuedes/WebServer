#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <cstdint>
#include <string>

class HttpResponse {
  public:
    HttpResponse(void);
    HttpResponse(HttpResponse const &cpy);
    ~HttpResponse(void);
    HttpResponse      &operator=(HttpResponse const &rhs);
    std::string        createSerializedResponse(void) const;
    std::string        createStatusLine(void) const;
    std::string        getServer(void) const;
    static std::string getReasonPhrase(int key);

  private:
    uint8_t _status_code;
};

#endif
