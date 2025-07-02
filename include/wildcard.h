#ifndef WILDCARD_H
#define WILDCARD_H

#include "../_libft/libft.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>


#define R_FAIL 0
#define R_SUCCESS 1
#define SINGLE_QUOTE 14
#define DOUBLE_QUOTE 15


typedef struct s_wc_node
{
    char            *data;
    int             match;
    struct s_wc_node   *next;
	struct s_wc_node   *prev;
}   t_wc_node;

//utils
int		get_wc_list_len(t_wc_node *list);
void	cleanup_wc_list_shels(t_wc_node *list);
void	assign_wc_node_data_to_args_array(t_wc_node *list, char ***arr);
int		append_wc_node(t_wc_node **list, char *data, int flag);
int		wildcard(char ***args);
int		init_file_names(t_wc_node **file_names);
void	free_wc_node_list(t_wc_node *list);
int		wildcard(char ***args);
int		expand_wildcard(t_wc_node **args_list, t_wc_node *file_names);

#endif