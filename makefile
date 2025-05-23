CFLAGS =  #-fsanitize=address -g3 #-Wall -Wextra -Werror

NAME = minishell

SRC =	minishell.c parsing/parser/parser.c parsing/tokenizer.c parsing/parser/tree.c \
		parsing/parser/clear_parsing.c parsing/parser/debug.c  parsing/parser/redirections.c \
		parsing/commands.c\
		parsing/tree/append_flat_tree_node.c parsing/tree/flat_tree.c\
		parsing/split/split.c parsing/split/manage_normal.c parsing/split/manage_operators.c parsing/split/manage_parenth.c parsing/split/manage_quotes.c parsing/split/manage_redirections.c parsing/split/append_node.c\
 		parsing/expantion/expand.c parsing/expantion/expand_utils.c parsing/expantion/expand_redirections.c parsing/expantion/do_expantion.c parsing/expantion/trim_quotes.c\
		parsing/utils/get_list_size.c\
		parsing/validate_syntax/validate_syntax.c parsing/validate_syntax/validate_quotes.c parsing/validate_syntax/validate_parenthecies.c\
		env/env.c env/env_utils.c\
		execution/execution.c execution/built-in/builtin_echo.c \
		execution/built-in/builtin_cd.c execution/built-in/builtin_env.c \
		execution/built-in/builtin_exit.c  execution/built-in/builtin_export.c \
		execution/built-in/builtin_pwd.c execution/built-in/builtin_unset.c \
		execution/path_utils.c 
		# debug / memory_debugging.c

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

debug: fclean all #todo 
	valgrind --leak-check=full --track-fds=yes ./minishell 