#pragma once
#ifndef CGI_HPP
# define CGI_HPP
# include "ISocket.hpp"

class Cgi : public ISocket
{
	public:
		Cgi(void);
		Cgi(Cgi const &cpy);
		~Cgi(void);
		Cgi&	operator=(Cgi const &rhs);
		int	getFd(void);
	
	private:
		int	_fd;
};

#endif //!CGI_HPP
