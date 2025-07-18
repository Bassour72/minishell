/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:02:16 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 16:11:45 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include <stdio.h>
# include "env.h"
# include "../_libft/libft.h"

# define R_FAIL 0
# define R_SUCCESS 1
# define SINGLE_QUOTE 14
# define DOUBLE_QUOTE 15
# define R_CONTINUE 2
# define R_DONE 3

typedef enum e_expand_type
{
	VAR,
	e_WORD
}				t_expand_type;

typedef struct s_expand_token
{
	char					*data;
	int						join;
	struct s_expand_token	*next;
	struct s_expand_token	*prev;
}				t_expand_token;

typedef struct s_expand_node
{
	char					*data;
	int						joinable;
	struct s_expand_node	*next;
}			t_expand_node;

typedef struct s_data_between_functions
{
	t_expand_token	**tokens;
	t_env			*env;
	char			*str;
	char			**data;
	int				*i;
	int				*quote;
}				t_expand_funcs_data;

int		expand(char ***new_args, t_env *env);
int		array_to_list(char **arr, t_node **list);
int		tokenize(char *str, t_expand_token **tokens, t_env *env);
int		join_arr(char **arr, char **joined_arr);
int		append_expand_token(t_expand_token **tokens, char *data, int join);
void	free_expand_tokens_list(t_expand_token *tokens);
void	print_expand_tokens(t_expand_token *tokens);
int		expand_list_to_array(char ***new_args, t_node *splited_line);
int		split_tokens_into_nodes(t_expand_node **expanded_list, \
t_expand_token *tokens);
int		is_valid_key_char(char c, int i);
int		extract_var_value(char *src, int *i, char **dst, t_env *env);
void	free_expand_list_nodes(t_expand_node *list);
int		single_quote_expander(char *str, int *i, t_expand_token **tokens);
int		double_quote_expander(char *str, int *i, t_expand_token **tokens, \
t_env *env);
int		normal_expander(char *str, int *i, t_expand_token **tokens);
int		normal_variable_expander(char *str, int *i, t_expand_token **tokens, \
t_env *env);
int		double_quote_variable_expander(char *str, int *_i, char **data, \
t_env *env);
int		expand_tokens_to_line(char **new_line, t_expand_token *tokens);
int		is_befor_var_valid(t_expand_token *tokens);

#endif