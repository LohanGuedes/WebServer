#include "RegularRequest.hpp"
#include "AHttpRequest.hpp"

RegularRequest::RegularRequest(Client const *const clientInstance)
    : AHttpRequest(clientInstance) {}

RegularRequest::~RegularRequest(void) {}
