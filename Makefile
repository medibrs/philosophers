NAME = philosophers
SRC = main.c philosopher.c utils.c monitor.c
CC = gcc 
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

