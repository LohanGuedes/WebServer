#include "MultipartRequest.hpp"
#include "Logger.hpp"

MultipartRequest::MultipartRequest(Client *const clientInstance)
    : AHttpRequest(clientInstance) {}

MultipartRequest::~MultipartRequest(void) {}

void MultipartRequest::processRequest(void) throw() {
    Logger::log(LOG_INFO, "MultipartRequest::processRequest() called");
    // TODO: implement processRequest
}
