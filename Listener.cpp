#include "Listener.hpp"

Listener::Listener(void)
{
}

Listener::Listener(Listener const &cpy)
{
}

Listener::~Listener(void)
{
}

Listener	&Listener::operator=(Listener const &rhs)
{
}

int	Listener::getFd(void) { return(this->_fd); }
