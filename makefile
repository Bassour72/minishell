CFLAGS =  -fsanitize=address -g3 #-Wall -Wextra -Werror

NAME = minishell

SRC =	minishell.c parsing/parser.c parsing/tokenizer.c parsing/tree.c parsing/commands.c parsing/split.c \
		parsing/expantion/extand.c \
		parsing/validate_syntax/validate_syntax.c\
		env/env.c\
		execution/execution.c execution/built-in/builtin_echo.c \
		execution/built-in/builtin_cd.c execution/built-in/builtin_env.c \
		execution/built-in/builtin_exit.c  execution/built-in/builtin_export.c \
		execution/built-in/builtin_pwd.c execution/built-in/builtin_unset.c

OBJ = $(SRC:.c=.o)

_libft = _libft/libft.a

all: libft $(NAME)

$(NAME): $(OBJ) 
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
