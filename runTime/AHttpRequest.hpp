#ifndef AHTTPREQUEST_HPP
#define AHTTPREQUEST_HPP

class Client; // forward declaration

class AHttpRequest {
  public:
    AHttpRequest(Client const *const clientInstance);
    virtual ~AHttpRequest(void);
    virtual void processRequest(void) throw() = 0;

  protected:
    Client const *const _clientInstance;
};

#endif // !AHTTPREQUEST_HPP
