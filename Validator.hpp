#pragma once

#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP
# include <netdb.h>
# include <cstddef>

class Validator
{
	public:
		static bool	validateTcpIpAvailable(void);
	private:
		Validator(void);
		~Validator(void);
};

#endif // !VALIDATOR_HPP
