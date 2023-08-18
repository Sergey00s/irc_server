NAME=bot

SRCS=$(wildcard *.cpp)


all: $(NAME)


$(NAME): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(NAME)


clean:
	rm -f $(NAME)

re: clean all

.PHONY: all clean re