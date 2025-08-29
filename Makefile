NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

SRC = src/main.c src/parsing.c src/parsing_utils.c src/init.c src/simulation.c src/actions.c src/actions_extra.c src/monitor.c src/utils.c src/utils_extra.c

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	@rm -f $(NAME)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re