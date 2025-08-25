NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCDIR = src
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/parsing.c $(SRCDIR)/parsing_utils.c $(SRCDIR)/init.c $(SRCDIR)/simulation.c $(SRCDIR)/actions.c $(SRCDIR)/actions_extra.c $(SRCDIR)/monitor.c $(SRCDIR)/utils.c $(SRCDIR)/utils_extra.c

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re