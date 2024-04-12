#include "AHttpRequest.hpp"

AHttpRequest::AHttpRequest(Client const *const clientInstance)
    : _clientInstance(clientInstance) {}

AHttpRequest::~AHttpRequest(void) {}
