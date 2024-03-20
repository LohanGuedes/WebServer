#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ISocket.hpp"

class Client : public ISocket
{
	public:
		Client(void);
		Client(Client const &cpy);
		~Client(void);
		Client&	operator=(Client const &rhs);

		virtual void handlePoll(POLL_VALUES value);
	private:
		virtual void handlePollout(POLL_VALUES value);
		virtual void handlePollin(void);
};

#endif // !CLIENT_HPP
