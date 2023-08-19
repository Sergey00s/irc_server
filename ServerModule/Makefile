NAME=libserver.a
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
CP = g++ -std=c++98 -Wall -Wextra -Werror
RM = rm -f


all: $(NAME)


$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)


%.o: %.cpp
	$(CP) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re