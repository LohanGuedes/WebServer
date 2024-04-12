#include "MultipartRequest.hpp"
#include "AHttpRequest.hpp"

MultipartRequest::MultipartRequest(Client const *const clientInstance)
    : AHttpRequest(clientInstance) {}

MultipartRequest::~MultipartRequest(void) {}
