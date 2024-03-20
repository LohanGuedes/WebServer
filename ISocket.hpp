#pragma once
#ifndef ISOCKET_HPP
# define ISOCKET_HPP

enum POLL_VALUES {

};

class ISocket
{
	public:
		ISocket(void);
		virtual ~ISocket(void);
		virtual void handlePoll(POLL_VALUES value) = 0;
	protected:
		virtual void handlePollin(POLL_VALUES value) = 0;
		virtual void handlePollout(void) = 0;
};

#endif // !ISOCKET_HPP
