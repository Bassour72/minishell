#ifndef EXPAND_H
#define EXPAND_H

#include <stdio.h>
#include "env.h"
#include "../_libft/libft.h"

#define R_FAIL 0
#define R_SUCCESS 1

typedef enum	e_expand_type
{
	VAR, e_WORD
}				t_expand_type;


typedef struct	s_node
{
	char			*data;
	struct s_node	*next;
}				t_node;

typedef struct	s_expand_token
{
	char					*data;
	int						join;
	int						split;
	struct s_expand_token	*next;
	struct s_expand_token	*prev;
}				t_expand_token;

typedef struct s_expand_node
{
	char					*data;
	int 					joinable;
	struct s_expand_node	*next;
}			t_expand_node;



int expand(char ***new_args, char **old_args, t_env *env);
int array_to_list(char **arr, t_node **list);
int tokenize(char *str, t_expand_token **tokens, t_env *env);
int join_arr(char **arr, char **joined_arr);
int append_expand_token(t_expand_token **tokens,\
char *data, t_expand_type type, int need_split, int join);

void free_expand_tokens_list(t_expand_token *tokens);
void print_expand_tokens(t_expand_token *tokens);

t_expand_node	*expand_split(char const *s, char c, int joinable);
int build_args_list_from_nodes_by_joining(t_expand_node *list);
int expand_list_to_array(char ***commands_arr, t_expand_node *list);


void free_expand_list_nodes(t_expand_node *list);
#endif