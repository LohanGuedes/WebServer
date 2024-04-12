#include "RegularRequest.hpp"

RegularRequest::RegularRequest(Client const *const clientInstance)
    : AHttpRequest(clientInstance) {}

RegularRequest::~RegularRequest(void) {}

void RegularRequest::processRequest(void) throw() {
    // TODO: implement processRequest
}
