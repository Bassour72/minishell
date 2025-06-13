#include "../../include/parsing.h"

static void move_the_loop(t_expand_node **tmp)
{
	t_expand_node *tmp_node;

	tmp_node = *tmp;
	*tmp = (*tmp)->next;
	free(tmp_node->data);
	free(tmp_node);
}

int join_joinable_nodes(t_expand_node *list, t_expand_node **current)
{
	t_expand_node *tmp;
	char *tmp_data;

	tmp = (*current)->next;
	while (tmp)
	{
		tmp_data = ft_strjoin((*current)->data, tmp->data);
		// printf("now[%s] next[%s]\n", (*current)->data, tmp->data);
		if (!tmp_data)
			return (perror("error: "), R_FAIL);
		free((*current)->data);
		(*current)->data = tmp_data;
		(*current)->next = tmp->next;

		if (tmp->joinable == 0)
		{
			move_the_loop(&tmp);
			break;
		}
		move_the_loop(&tmp);
	}
	*current = tmp;
	return (R_SUCCESS);
}

int build_args_list_from_nodes_by_joining(t_expand_node *list)
{
    t_expand_node	*tmp;
	t_expand_node	*current;
	t_expand_node	*tmp_node;;
	char			*tmp_data;
	// printf("=enter=======================================\n");
    current = list;
    while (current)
    {
		if (current->joinable == 1)
		{
			// printf("join---------[%s]----\n", current->data);
			if (join_joinable_nodes(list, &current) == R_FAIL)
			{
				return (R_FAIL);
			}
		}
		else
		{
			// printf("else---------[%s]----\n", current->data);
			current = current->next;
		}
    }
	// printf("=exit=======================================\n");
	// for(t_expand_node *tmp = list; tmp; tmp = tmp->next)
	// 	printf("\033[34m%s\033[0m\n", tmp->data);
	return (R_SUCCESS);
}
