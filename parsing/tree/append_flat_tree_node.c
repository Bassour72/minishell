#include "../../include/parsing.h"


static t_flat_tree *create_flat_tree_list(t_flat_tree *flat_tree_list, t_tree *tree_node)
{
	t_flat_tree	*new_flat_tree_list;

	new_flat_tree_list = malloc(sizeof(t_flat_tree));
	if (!new_flat_tree_list)
	{
		free_tree_node(tree_node);//note if malloc failed free the given tree_node
		return (NULL);
	}
	new_flat_tree_list->tree_node = tree_node;
	new_flat_tree_list->next = NULL;
	new_flat_tree_list->backup_next = NULL;
	new_flat_tree_list->prev = NULL;
	return (new_flat_tree_list);
}

static t_flat_tree *append_node_to_flat_tree_list(t_flat_tree *flat_tree_list, t_tree *tree_node)
{
	t_flat_tree *tmp;

	tmp = flat_tree_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_flat_tree));
	if (!tmp->next)
	{
		free_tree_node(tree_node);//note if malloc failed free the given tree_node and the pre allocated flat_tree_list
		free_flat_tree_list(flat_tree_list);
		return (NULL);
	}
	tmp->next->tree_node = tree_node;
	tmp->next->next = NULL;
	tmp->next->backup_next = NULL;
	tmp->next->prev = tmp;

	tmp->backup_next = tmp->next;
	// printf("append %p -> %p \n", tmp, tree_node);
	return (flat_tree_list);
}

t_flat_tree *append_new_flat_tree_node(t_flat_tree *flat_tree_list, t_tree *tree_node)
{
	t_flat_tree	*new_flat_tree_list;
	t_flat_tree	*tmp;
	
	//note create new flat tree
	if (!flat_tree_list)
		return (create_flat_tree_list(flat_tree_list, tree_node));
	else
		return (append_node_to_flat_tree_list(flat_tree_list, tree_node));
}