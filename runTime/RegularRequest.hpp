#ifndef REGULARREQUEST_HPP
# define REGULARREQUEST_HPP

class RegularRequest
{
	public:
		RegularRequest(void);
		RegularRequest(RegularRequest const &cpy);
		~RegularRequest(void);
		RegularRequest&	operator=(RegularRequest const &rhs);
};

#endif
