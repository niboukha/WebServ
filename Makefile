NAME = WebServ

RM = rm -f

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g3

CXX = c++

SRCS =   main.cpp  ConfigFile.cpp Server.cpp  \
		StringOperations.cpp Request.cpp Client.cpp

OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $<

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all