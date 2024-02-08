NAME = WebServ

RM = rm -f

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror 

CXX = c++

SRCS =   main.cpp  parseConfigFile.cpp Server.cpp Location.cpp StringOperations.cpp

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