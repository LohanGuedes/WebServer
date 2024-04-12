NAME = test 


SRCS =	main.cpp \
		Logger.cpp \
		RunTime.cpp \
		Listener.cpp \
		Client.cpp \
		AHttpRequest.cpp \
		RegularRequest.cpp \
		MultipartRequest.cpp \
		CgiRequest.cpp \


DEPS =	$(SRCS) \
		Logger.hpp \
		RunTime.hpp \
		APollable.hpp \
		Listener.hpp \
		Client.hpp \
		AHttpRequest.hpp \
		RegularRequest.hpp \
		MultipartRequest.hpp \
		CgiRequest.hpp \

CC = c++

CFLAGS = -g -Wall -Wextra -Werror -std=c++98

.PHONY: all
all: $(NAME)

$(NAME): $(DEPS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

.PHONY: clean 
clean:
	rm -f $(NAME)

.PHONY: re
re: clean all
