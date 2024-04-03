NAME = test 


SRCS =	main.cpp \
		RunTime.cpp \
		Listener.cpp \


DEPS =	$(SRCS) \
		RunTime.hpp \
		ISocket.hpp \
		Listener.hpp \

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
