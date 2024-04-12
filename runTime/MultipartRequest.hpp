#ifndef MULTIPARTREQUEST_HPP
# define MULTIPARTREQUEST_HPP

class MultipartRequest
{
	public:
		MultipartRequest(void);
		MultipartRequest(MultipartRequest const &cpy);
		~MultipartRequest(void);
		MultipartRequest&	operator=(MultipartRequest const &rhs);
};

#endif
