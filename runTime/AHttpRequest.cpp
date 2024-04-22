#include "AHttpRequest.hpp"
#include "Client.hpp"
#include "HttpResponse.hpp"
#include "RunTime.hpp"

AHttpRequest::AHttpRequest(Client *const clientInstance)
    : _clientInstance(clientInstance) {}

AHttpRequest::~AHttpRequest(void) {}

int AHttpRequest::notifyResponseReady(HttpResponse *const res) {
    // TODO: Implement this method

    if (!res) {
        return (-1);
    }

    this->_clientInstance->response = res;
    if (this->_clientInstance->pollout_ready != true) {
        return (1);
    }
    this->_clientInstance->sendResponse();
    RunTime *const rt = RunTime::getInstance();
    if (!rt) {
        // TODO: Implement analogous to panic!
    }
    rt->deleteClient(this->_clientInstance);
    return (0);
}
