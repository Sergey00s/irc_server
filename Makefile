NAME=irc
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
CP = g++ -std=c++98 #-Wall -Wextra -Werror 
RM = rm -f
LIBSERV = ServerModule/libserver.a

all:  $(LIBSERV) $(NAME)

$(LIBSERV):
	make -C ServerModule

$(NAME): $(OBJS)
	$(CP) $(OBJS) -o $(NAME) $(LIBSERV)

%.o: %.cpp
	$(CP) -c $< -o $@ -I ServerModule

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
ffclean: fclean
	make -C ServerModule fclean

re: ffclean all

.PHONY: all clean fclean re


