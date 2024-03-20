#include "Client.hpp"

Client::Client(void)
{
}

Client::Client(Client const &cpy)
{
}

Client::~Client(void)
{
}

Client	&Client::operator=(Client const &rhs)
{
}

int	Client::getFd(void) { return(this->_fd); }
