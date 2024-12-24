NAME = philosophers
SRC = main.c philosopher.c utils.c monitor.c 
CC = gcc # -fsanitize=thread -g3
CFLAGS = -Wall -Wextra -Werror 
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

