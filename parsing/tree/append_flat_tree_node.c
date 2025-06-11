#include "../../include/parsing.h"


static int	create_flat_tree_list(t_flat_tree **flat_tree_list, t_tree *tree_node)
{


	*flat_tree_list = malloc(sizeof(t_flat_tree));
	if (!*flat_tree_list)
	{
		free_tree_node(tree_node);//note if malloc failed free the given tree_node
		return (R_FAIL);
	}
	(*flat_tree_list)->tree_node = tree_node;
	(*flat_tree_list)->next = NULL;
	(*flat_tree_list)->backup_next = NULL;
	(*flat_tree_list)->prev = NULL;
	return (R_SUCCESS);
}

static int	append_node_to_flat_tree_list(t_flat_tree **flat_tree_list, t_tree *tree_node)
{
	t_flat_tree *tmp;

	tmp = *flat_tree_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next =NULL;// malloc(sizeof(t_flat_tree));
	if (!tmp->next)
	{
		free_tree_node(tree_node);//note if malloc failed free the given tree_node and the pre allocated flat_tree_list
		free_flat_tree_list(*flat_tree_list);
		return (R_FAIL);
	}
	tmp->next->tree_node = tree_node;
	tmp->next->next = NULL;
	tmp->next->backup_next = NULL;
	tmp->next->prev = tmp;

	tmp->backup_next = tmp->next;
	// printf("append %p -> %p \n", tmp, tree_node);
	return (R_SUCCESS);
}

int	append_new_flat_tree_node(t_flat_tree **flat_tree_list, t_tree *tree_node)
{
	
	//note create new flat tree
	if (!*flat_tree_list)
		return (create_flat_tree_list(flat_tree_list, tree_node));
	else
		return (append_node_to_flat_tree_list(flat_tree_list, tree_node));
}