/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:38:44 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 15:01:55 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

# include "../_libft/libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
# include <stdbool.h>

# define R_FAIL 0
# define R_SUCCESS 1
# define R_CONTINUE 2
# define SINGLE_QUOTE 14
# define DOUBLE_QUOTE 15

typedef struct s_wc_node
{
	char				*data;
	int					match;
	int					private;
	struct s_wc_node	*next;
	struct s_wc_node	*prev;
}				t_wc_node;

typedef struct s_is_match_args
{
	int	i;
	int	wc_pos;
	int	j;
	int	file_name_len;
	int	str_len;
	int	is_literal_string;
}				t_is_match_args;

int			get_wc_list_len(t_wc_node *list);
void		cleanup_wc_list_shels(t_wc_node *list);
void		assign_wc_node_data_to_args_array(t_wc_node *list, char ***arr);
int			append_wc_node(t_wc_node **list, char *data, int flag);
int			wildcard(char ***args);
int			init_file_names(t_wc_node **file_names);
void		free_wc_node_list(t_wc_node *list);
int			wildcard(char ***args);
int			expand_wildcard(t_wc_node **args_list, t_wc_node *file_names);
int			is_wildcard(char *str);
int			init_needle_len(char *name, int start);
int			is_match_quote(char str_i, char file_name_j);
t_wc_node	*sort_filenames(t_wc_node *env_list);
void		init_file_names_matches(t_wc_node *file_names, char *name);

#endif