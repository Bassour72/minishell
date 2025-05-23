#include "../../../include/expand.h"

void free_expand_list_nodes(t_expand_node *list)
{
	t_expand_node *tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}