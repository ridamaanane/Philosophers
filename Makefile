CC = cc

CFLAGS= -Wall -Wextra -Werror

SRC = parse.c philo.c routine.c mounitor.c utils.c

OBJ = $(SRC:%.c=%.o)

NAME = philo

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean  all

.SECONDARY: $(OBJ)
.PHONY: re fclaen clean all