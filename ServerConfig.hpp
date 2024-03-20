#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP
# include <iostream>
#include <string>

class ServerConfig
{
	public:
		ServerConfig(void);
		ServerConfig(ServerConfig const &cpy);
		~ServerConfig(void);
		ServerConfig&	operator=(ServerConfig const &rhs);

		std::string		hostPort; // e.g: "localhost:4040"
		std::string		serverName;
};

#endif
