CFLAGS =  -fsanitize=address -g3 #-Wall -Wextra -Werror

NAME = expand
_libft = _libft/libft.a 
 

SRC = main.c env/env_utils.c env/env.c expand.c \
	tokenizer/tokenizer.c tokenizer/append_expand_token.c tokenizer/free_expand_tokens_list.c tokenizer/print_expand_tokens.c\
	utils/array_to_list.c utils/join_array.c\
	expand_split.c\
	build_args_list.c\
	expand_list_to_array.c\

OBJ = $(SRC:.c=.o)

all: libft $(NAME)

libft:
	make -C ./_libft

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) $(_libft) -o $@


%.o: %.c  expand.h env/env.h _libft/libft.h
	cc $(CFLAGS)  -c $< -o $@

clean:   
	make clean -C ./_libft
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all