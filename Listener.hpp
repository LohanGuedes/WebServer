#pragma once
#ifndef LISTENER_HPP
# define LISTENER_HPP

# include <vector>
# include "ISocket.hpp"
# include "ServerConfig.hpp"

class Listener : public ISocket
{
	public:
		Listener(void);
		Listener(Listener const &cpy);
		virtual ~Listener(void);
		Listener&	operator=(Listener const &rhs);
		virtual void handlePoll(POLL_VALUES value);

		std::vector<ServerConfig>		configPool;
		static std::vector<Listener *>	listenerPool;

	private:
		virtual void handlePollin(POLL_VALUES value);
		virtual void handlePollout(void);
};

#endif // !LISTENER_HPP
