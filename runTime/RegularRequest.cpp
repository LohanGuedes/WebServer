#include "RegularRequest.hpp"

RegularRequest::RegularRequest(Client const *const clientInstance)
    : AHttpRequest(clientInstance) {}

RegularRequest::~RegularRequest(void) {}
