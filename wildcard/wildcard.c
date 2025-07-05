
#include "../include/wildcard.h"

void free_wc_node_list(t_wc_node *list)
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
        if (append_wc_node(list, args[i], 0) == R_FAIL)
            return (free_wc_node_list(*list), R_FAIL);
    }
    return (R_SUCCESS);
}

int wildcard(char ***args)
{
    t_wc_node	*file_names;
    t_wc_node	*args_list;
	int			len;
    int         state;

    if (!args)
        return (R_SUCCESS);
    file_names = NULL;
    args_list = NULL;
    state = init_file_names(&file_names);
    if (state == R_CONTINUE)
        return (R_CONTINUE);
    else if (state == R_FAIL)
        return (R_FAIL);
    else if(state == 2)
        return (R_SUCCESS);
    
    if (init_args_list(&args_list, *args) == R_FAIL)
        return (free_2d_arr(*args), free_wc_node_list(file_names), R_FAIL);
	free_2d_arr(*args);
    if (expand_wildcard(&args_list, file_names) == R_FAIL)
        return (free_wc_node_list(file_names), free_wc_node_list(args_list), R_FAIL);
	free_wc_node_list(file_names);
	len = get_wc_list_len(args_list);
	*args = malloc(sizeof(char *) * (len + 1));
	if (!*args)
		return (perror("error: "), free_wc_node_list(args_list), R_FAIL);
	assign_wc_node_data_to_args_array(args_list, args);
	cleanup_wc_list_shels(args_list);
    return (R_SUCCESS);
}
