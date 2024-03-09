CXX = c++

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g3

NAME = webserv

SRC = Delete/Delete.cpp Post/Post.cpp Request/Request.cpp Response/Response.cpp Server/Server.cpp main.cpp\
	Get/Get.cpp Utils/Utils.cpp Client/Client.cpp ConfigFile/ConfigFile.cpp Utils/StringOperations.cpp\
	Multiplexer/Multiplexer.cpp Cgi/Cgi.cpp

OBJ = $(SRC:%.cpp=%.o)

all : $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

#%.o : %.cpp %.hpp
#$(CXX) $(CXXFLAGS) -c $<

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re
