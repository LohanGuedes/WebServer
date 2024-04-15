#include "RegularRequest.hpp"
#include "Logger.hpp"

RegularRequest::RegularRequest(Client const *const clientInstance)
    : AHttpRequest(clientInstance) {}

RegularRequest::~RegularRequest(void) {}

void RegularRequest::processRequest(void) throw() {
    Logger::log(LOG_INFO, "RegularRequest::processRequest() called");
    // TODO: implement processRequest
}
