
#include "../include/wildcard.h"

void free_node_list(t_wc_node *list)
{
    t_wc_node *tmp;

    while (list)
    {
        tmp = list;
        list = list->next;
        free(tmp->data);
        free(tmp);
    }
}

int init_args_list(t_wc_node **list, char **args)
{
    int i;

    i = -1;
    while (args[++i])
    {
        if (append_wc_node(list, args[i]) == R_FAIL)
            return (free_node_list(*list), R_FAIL);
    }
    return (R_SUCCESS);
}

int wildcard(char ***args)
{
    t_wc_node *file_names;
    t_wc_node *args_list;

    if (!args)
        return (R_SUCCESS);
    file_names = NULL;
    args_list = NULL;
    if (init_file_names(&file_names) == R_FAIL )
        return (R_FAIL);

    if (init_args_list(&args_list, *args) == R_FAIL)
        return (free_node_list(file_names), R_FAIL);

	
    if (expand_wildcard(&args_list, file_names) == R_FAIL)
	{
        return (free_node_list(file_names), free_node_list(args_list), R_FAIL);
	}
	free_node_list(file_names);
    
	for(int i = 0; (*args)[i]; i++)
	{
		free((*args)[i]);
	}
	free(*args);

	int len  = 0;
	for(t_wc_node *t = args_list; t; t = t->next)
		len++;
	*args = malloc(sizeof(char *) * (len + 1));
	if (!*args)
		return (perror("error: "), free_node_list(args_list), R_FAIL);
	int i = 0;
	for(t_wc_node *t = args_list; t; t = t->next)
	{
		(*args)[i++] = t->data; 
	}
	(*args)[len]=  NULL;
	t_wc_node *tmp;
	while (args_list)
	{
		tmp = args_list;
		args_list = args_list->next;
		free(tmp);
	}
    return (R_SUCCESS);
}
