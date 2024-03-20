#include "Validator.hpp"

Validator::Validator(void) { }

Validator::~Validator(void) { }

bool	Validator::validateTcpIpAvailable(void)
{
	return (getprotobyname("tcp") != NULL && getprotobyname("ip") != NULL);
}
