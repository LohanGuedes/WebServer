#include "CgiRequest.hpp"
#include "Logger.hpp"

CgiRequest::CgiRequest(Client const *const clientInstance)
    : AHttpRequest(clientInstance) {}

CgiRequest::~CgiRequest(void) {}

void CgiRequest::processRequest(void) throw() {
    Logger::log(LOG_INFO, "CgiRequest::processRequest() called");
    // TODO: implement processRequest
}
