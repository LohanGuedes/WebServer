#include "CgiRequest.hpp"

CgiRequest::CgiRequest(Client const *const clientInstance)
    : AHttpRequest(clientInstance) {}

CgiRequest::~CgiRequest(void) {}

void CgiRequest::processRequest(void) throw() {
    // TODO: Implement processRequest
}
