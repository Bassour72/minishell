CFLAGS = -fsanitize=address -g3 #-Wall -Wextra -Werror

NAME = minishell

SRC =	minishell.c parsing/parser/parser.c parsing/tokenizer.c parsing/parser/tree.c \
		parsing/parser/clear_parsing.c parsing/parser/debug.c  parsing/parser/redirections.c \
		parsing/commands.c\
		parsing/tree/append_flat_tree_node.c parsing/tree/flat_tree.c\
		parsing/split/split.c parsing/split/manage_normal.c parsing/split/manage_operators.c parsing/split/manage_parenth.c parsing/split/manage_quotes.c parsing/split/manage_redirections.c parsing/split/append_node.c\
 		parsing/expand/expand.c \
		parsing/expand/remove_non_printable_characters.c\
		parsing/expand/expand_node.c \
		parsing/expand/utils/print.c\
		parsing/expand/tokenizer/single_quote_expander.c\
		parsing/expand/tokenizer/double_quote_expander.c\
		parsing/expand/tokenizer/variable_expander.c\
		parsing/expand/tokenizer/normal_expander.c\
		parsing/expand/tokenizer/normal_variable_expander.c\
		parsing/expand/tokenizer/double_quote_variable_expander.c\
		parsing/expand/tokenizer/tokenizer.c \
		parsing/expand/tokenizer/append_expand_token.c \
		parsing/expand/tokenizer/free_expand_tokens_list.c \
		parsing/expand/tokenizer/print_expand_tokens.c\
		parsing/expand/utils/array_to_list.c \
		parsing/expand/utils/join_array.c\
		parsing/expand/expand_split.c\
		parsing/expand/build_args_list.c\
		parsing/expand/expand_list_to_array.c\
		parsing/utils/get_list_size.c\
		parsing/validate_syntax/validate_syntax.c parsing/validate_syntax/validate_quotes.c parsing/validate_syntax/validate_parenthecies.c\
		wildcard/expand_wildcard.c\
		wildcard/init_file_names.c\
		wildcard/wildcard.c\
		env/env.c env/env_utils.c\
		execution/execution.c execution/built-in/builtin_echo.c \
		execution/built-in/builtin_cd.c execution/built-in/builtin_env.c \
		execution/built-in/builtin_exit.c  execution/built-in/builtin_export.c \
		execution/built-in/builtin_pwd.c execution/built-in/builtin_unset.c \
		execution/path_utils.c \
		execution/redir_utils.c \
		execution/shell_levl/shlvl.c \
		execution/built-in/execute_builtin.c \
		execution/exec/exec_cmd.c \
		execution/env/env_array.c
		# debug / memory_debugging.c 

OBJ = $(SRC:.c=.o)

_libft = _libft/libft.a

all: libft $(NAME)

$(NAME): $(OBJ) 
	cc $(CFLAGS) $(OBJ) $(_libft)  -lreadline -o $@

libft:
	make -C ./_libft

%.o: %.c minishell.c include/*.h
	cc $(CFLAGS) -c $< -o $@

clean:   
	make clean -C ./_libft
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all



debug: fclean all #todo 
	valgrind --leak-check=full --track-fds=yes ./minishell 

debugg: fclean all #todo 
	valgrind --leak-check=full --show-leak-kinds=all ./minishell 
de:
	valgrind --leak-check=full --track-fds=yes bash 
debug_mode_min:
	valgrind --leak-check=full --show-leak-kinds=all ./minishell

debug_mode_all:
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes ./minishell




#ls | ls | l | l | l | l | l || l | l | ls -al && ls | cat
#  lss || ls && s || ls -al && sls || ls -al | ls
# s | l | lS | ls | ls | ls -al | l | l | l | l | ls | l | l || l || l || l | l | l 
#s | l | l | l | l | l | l | l | l | l | l | l | l || l || l || l | l | l
#  s | l | l | l | l | l | l | l | l | l | l | l | l || l || l || l | l | l > younes.txt
#bash -x -c '(ls && ls) && lss || ls | cat && grep makefile'
# (ls && ls ) && lss || ls | cat | grep makefile 
# (ls && ls) && lss || ls | cat && grep makefile
# (ls && ls) && lss || ls | cat  
#minishell$ '' ' '''
# export b="1 2 3" c="1 2 3"
# export $cc=$cc
#export x='h j k'
# export $x=$x

### bash-5.2$ ls || |
### bash: syntax error near unexpected token `|'
### bash-5.2$ echo $?
### 2
# cat >> $?vv
# ls | ls | ls |cat | grep m | wc -w | cat | ls | ls | ls | ls | ls | ls | ls | ls | ls -la && lsss | cat | grep m
#minishell$ ||
#minishell$ ||
#minishell$ ()
#minishell$ ((
#minishell$ export a="ls -la"
#minishell$ $a
#minishell: ls: Command not found: 
#minishell$ $aasd
# export )a
#bash: syntax error near unexpected token `)'
