NAME = webserv

SRCS = $(wildcard *.cpp)

DEPS =	$(SRCS) \
		$(wildcard *.hpp) \

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
