NAME = webserv
CC = c++
RM = rm -rf
FLAGS =  -Wall -Wextra -Werror -std=c++98
SRC =	webserv.cpp \
		main.cpp
OBJ = $(SRC:.c=.o)

%.o:%.cpp
	@$(CC) $(FLAGS) -c $<

all : $(NAME)

$(NAME) : $(OBJ)
	@$(CC) $(FLAGS) $^ -o $@

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all