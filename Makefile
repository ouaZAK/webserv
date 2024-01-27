NAME = webserv
CC = c++
RM = rm -rf
FLAGS =  -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g
SRC =	webserv.cpp \
		webInfo.cpp \
		Request.cpp \
		clientInfo.cpp \
		getServerInf.cpp \
		ServerInf.cpp \
		main.cpp
OBJ = $(SRC:.cpp=.o)

%.o:%.cpp
	@$(CC) $(FLAGS) -c $<

all : $(NAME)

$(NAME) : $(OBJ)
	@$(CC) $(FLAGS) $^ -o $@

clean :
	@$(RM) $(OBJ)

fclean : clean
	@$(RM) $(NAME)

re : fclean all