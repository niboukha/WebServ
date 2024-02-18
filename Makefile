CXX = c++

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g3

NAME = webserv

SRC = Delete.cpp Post.cpp Request.cpp Response.cpp Server.cpp main.cpp Get.cpp Utils.cpp Client.cpp

OBJ = $(SRC:%.cpp=%.o)

all : $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o : %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $<

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re
