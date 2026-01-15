NAME    = audioscope

CC      = gcc

CFLAGS  = -Wall -Wextra -Werror -Iinclude

IFLAGS = -Iinclude

SRC     = src/main.c \
          src/wav_reader.c \
          src/analysis.c

OBJ     = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(CFLAGS) $(IFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
