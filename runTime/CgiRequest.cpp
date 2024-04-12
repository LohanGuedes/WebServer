#include "CgiRequest.hpp"
#include "AHttpRequest.hpp"

CgiRequest::CgiRequest(Client const *const clientInstance)
    : AHttpRequest(clientInstance) {}

CgiRequest::~CgiRequest(void) {}
