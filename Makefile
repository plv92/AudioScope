NAME    = audioscope

CC      = gcc

CFLAGS  = -Wall -Wextra -Werror -Iinclude

IFLAGS = -Iinclude

CRITERION_FLAGS = $(shell pkg-config --cflags --libs criterion)
CFLAGS += -g --coverage
LDFLAGS += --coverage

SRC     = src/main.c \
          src/wav_reader.c \
          src/analysis.c \
		  src/open_file.c

OBJ     = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(CFLAGS) $(IFLAGS)


# Unit tests with Criterion
TESTS_SRC = $(wildcard tests/test_*.c)
TESTS_BIN = tests/unit_tests

tests: $(OBJ) $(TESTS_SRC)
	$(CC) $(CFLAGS) -o $(TESTS_BIN) $(TESTS_SRC) src/wav_reader.c src/open_file.c $(CRITERION_FLAGS) $(LDFLAGS)
	./$(TESTS_BIN)

clean_tests:
	rm -f $(TESTS_BIN)

coverage:
	gcovr --root . --object-directory tests --exclude-directories tests --html -o coverage.html

clean:
	rm -f $(OBJ)
	$(MAKE) clean_tests

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
