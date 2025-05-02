CFLAGS =  #-fsanitize=address -g3 #-Wall -Wextra -Werror

NAME = minishell

SRC =	minishell.c parsing/parser.c parsing/tokenizer.c parsing/tree.c parsing/commands.c parsing/split.c

OBJ = $(SRC:.c=.o)

_libft = _libft/libft.a

all: $(NAME)

$(NAME): $(OBJ) libft
	cc $(CFLAGS) $(OBJ) $(_libft)  -lreadline -o $@

libft:
	make -C ./_libft

%.o: %.c minishell.c include/parsing.h
	cc $(CFLAGS) -c $< -o $@

clean:
	make clean -C ./_libft
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
