
#include "../../include/parsing.h"


static int creat_the_list_from_zero(t_node **list, char **data)
{
	*list = malloc(sizeof(t_node));
	if (!*list)
		return (0);
	(*list)->data = ft_strdup(*data);
	if (!(*list)->data)
		return (0);
	(*list)->next = NULL;
	free(*data);
	*data = NULL;
	return (1);
}

static int append_new_node_to_existen_list(t_node **list, char **data)
{
	t_node *node;

	t_node *tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	node = malloc(sizeof(t_node));
	if (!node)
		return (0);
	node->data = ft_strdup(*data);
	if (!node->data)
		return (0);
	node->next = NULL;
	tmp->next = node;
	free(*data);
	*data = NULL;
	return (1);
}

int append_node(t_node **list, char **data)
{
	t_node *node;

	// printf("new_node_data [%s]\n", *data);

	// printf("data : {%s}\n", *data);
	if (!*list)
		return (creat_the_list_from_zero(list, data));
	else
		return (append_new_node_to_existen_list(list, data));
}