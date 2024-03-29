# add -Wall -wextra -werror  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
NAME= ./build/ircserv
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
CP = g++ -std=c++98 -Wall -Wextra -Werror 
RM = rm -f
LIBSERV = ServerModule/libserver.a
PROTOCOL = ProtocolModule/libprotocol.a

all: $(LIBSERV) $(PROTOCOL) $(NAME)

$(LIBSERV):
	make -C ServerModule

$(PROTOCOL):
	make -C ProtocolModule

$(NAME): $(OBJS)
	$(CP) $(OBJS) -o $(NAME) $(LIBSERV) $(PROTOCOL)

%.o: %.cpp
	$(CP) -c $< -o $@ -I ServerModule -I ProtocolModule

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

ffclean: fclean
	make -C ProtocolModule fclean
	make -C ServerModule fclean

re: ffclean all

.PHONY: all clean fclean re


