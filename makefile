CFLAGS = -Wall -Wextra -Werror

NAME = minishell

SRC =	minishell.c\
		minishell_helper.c\
		echoctl.c\
		parsing/commands.c\
		parsing/tokenizer.c\
		parsing/parser/parser.c\
		parsing/parser/init_tree.c\
		parsing/parser/clear_parsing.c\
		parsing/parser/redirections.c\
		parsing/tree/append_flat_tree_node.c\
		parsing/tree/flat_tree.c\
		parsing/tree/flat_tree_collectors.c\
		parsing/tree/flat_tree_utils.c\
		parsing/tree/parenths_block.c\
		parsing/split/split.c\
		parsing/split/manage_normal.c\
		parsing/split/manage_operators.c\
		parsing/split/manage_parenth.c\
		parsing/split/manage_quotes.c\
		parsing/split/manage_redirections.c\
		parsing/split/append_node.c\
		parsing/split/split_utils.c\
 		parsing/expand_split/expand_split.c\
		parsing/expand_split/manage_normal.c\
		parsing/expand/expand.c \
		parsing/expand/remove_non_printable_characters.c\
		parsing/expand/expand_herdoc.c \
		parsing/expand/expand_tokens_to_line.c\
		parsing/expand/tokenizer/variable_expander_utils.c\
		parsing/expand/tokenizer/single_quote_expander.c\
		parsing/expand/tokenizer/double_quote_expander.c\
		parsing/expand/tokenizer/finish_and_append_double_quote_expand.c\
		parsing/expand/tokenizer/normal_expander.c\
		parsing/expand/tokenizer/normal_variable_expander.c\
		parsing/expand/tokenizer/is_befor_var_valid.c\
		parsing/expand/tokenizer/double_quote_variable_expander.c\
		parsing/expand/tokenizer/tokenizer.c \
		parsing/expand/tokenizer/append_expand_token.c \
		parsing/expand/utils/free_expand_tokens_list.c\
		parsing/expand/utils/array_to_list.c \
		parsing/expand/utils/join_array.c\
		parsing/expand/expand_redirection.c\
		parsing/expand/delimiter_clear_dollar.c\
		parsing/expand/expand_herdoc_delimiter.c\
		parsing/expand/expand_list_to_array.c\
		parsing/utils/get_list_size.c\
		parsing/utils/tokenize_utils.c\
		parsing/validate_syntax/validate_syntax.c\
		parsing/validate_syntax/validate_quotes.c\
		parsing/validate_syntax/validate_syntax_utils.c\
		wildcard/init_filename_matches.c\
		wildcard/sort_filenames_list.c\
		wildcard/init_file_names.c\
		wildcard/expand_wildcard.c\
		wildcard/expand_wildcard_utils.c\
		wildcard/wildcard.c\
		wildcard/wildcard_utils.c\
		env/env.c\
		env/env_utils.c\
		env/init_env.c \
		env/create_env_node.c\
		env/set_pwd_and_oldpwd_if_not_found.c\
		env/update_exit_status.c\
		execution/execution.c \
		execution/built-in/unset/unset.c \
		execution/shell_levl/shlvl.c \
		execution/built-in/execute_builtin.c \
		execution/exec/exec_cmd.c \
		execution/env_array/env_array.c \
		execution/tree_execute/tree_exe_.c \
		execution/tree_execute/tree_exec_utils.c \
		execution/pipe_exec/exec_pipe.c \
		execution/pipe_exec/exec_pipe_utils.c \
		execution/handle_error_/handle_error_.c \
		execution/built-in/cd/cd.c \
		execution/built-in/cd/cd_arg_validation.c \
		execution/built-in/cd/cd_utils.c \
		execution/built-in/cd/cd_utils1.c \
		execution/built-in/cd/cd_utils2.c \
		execution/built-in/export/export.c \
		execution/built-in/export/export_utils2.c \
		execution/built-in/export/export_utils1.c \
		execution/built-in/export/export_utils.c  \
		execution/built-in/pwd/pwd.c \
		execution/built-in/exit/exit.c \
		execution/built-in/env/builtin_env.c \
		execution/built-in/echo/echo.c \
		execution/heredocs/heredocs_utils.c \
		execution/heredocs/heredocs.c \
		execution/path/path_utils.c \
		execution/path/path.c \
		execution/redirection_exec/redir_.c \
		execution/redirection_exec/redir_utils.c


OBJ = $(SRC:.c=.o)

READLINE_COMPILE = -I$(shell brew --prefix readline)/include
READLINE_LINK = -lreadline -L$(shell brew --prefix readline)/lib

_libft = _libft/libft.a

all: libft $(NAME)

$(NAME): $(OBJ) 
	cc $(CFLAGS) $(READLINE_COMPILE) $(OBJ) $(_libft) -o $@ $(READLINE_LINK)

libft:
	make -C ./_libft

%.o: %.c include/*.h _libft/libft.h _libft
	cc $(CFLAGS) $(READLINE_COMPILE) -c $< -o $@ 

clean:   
	make clean -C ./_libft
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
