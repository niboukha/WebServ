CXX = c++

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g3

NAME = webserv

SRC = ./sources/Delete.cpp ./sources/Post.cpp ./sources/Request.cpp ./sources/Response.cpp ./sources/Server.cpp ./sources/main.cpp\
	./sources/Get.cpp ./sources/Utils.cpp ./sources/Client.cpp ./sources/ConfigFile.cpp ./sources/StringOperations.cpp\
	./sources/Multiplexer.cpp

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
