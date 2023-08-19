NAME=bot

SRCS=$(wildcard *.cpp)


all: $(NAME)


$(NAME): $(SRCS)
	$(CXX) -Wall -Wextra -Werror $(SRCS) -o $(NAME)


clean:
	rm -f $(NAME)

re: clean all

.PHONY: all clean re