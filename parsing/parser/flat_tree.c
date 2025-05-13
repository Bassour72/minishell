#include "../../include/parsing.h"

t_flat_tree *flat_tree_last(t_flat_tree *flat_tree)
{
	while (flat_tree->next)
		flat_tree = flat_tree->next;
	return (flat_tree);
}


t_flat_tree *append_new_flat_tree_node(t_flat_tree *flat_tree_list, t_tree *tree_node)
{
	t_flat_tree	*new_flat_tree_list;
	t_flat_tree	*tmp;
	
	//note create new flat tree
	if (!flat_tree_list)
	{
		new_flat_tree_list = malloc(sizeof(t_flat_tree));
		if (!new_flat_tree_list)
		{
			free_tree_node(tree_node);//note if malloc failed free the given tree_node
			return (NULL);
		}
		new_flat_tree_list->tree_node = tree_node;
		new_flat_tree_list->next = NULL;
		new_flat_tree_list->prev = NULL;
		return (new_flat_tree_list);
	}
	
	//note append a new flat tree node to the flat tree list
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
	tmp->next->prev = tmp;
	// printf("append %p -> %p \n", tmp, tree_node);
	return (flat_tree_list);
}



t_flat_tree *create_flat_tree(t_token *token)
{
	t_flat_tree *flat_tree_list = NULL; //note linked list
	t_flat_tree *tmp; //note traverse the linked list
	t_tree *tree_node;  //note: tmp var 
	int i;
	int j; //note () reds index

	i = 0;
	while (token[i].data)
	{
	//LABLE PIPE / OPERATOR
		if (token[i].type == PIPE || token[i].type == OP_AND || token[i].type == OP_OR)
		{
			// printf("p/o %d \n", i);
			tree_node = new_tree_node(token[i].type);
			if (!tree_node)
				return (free_flat_tree(flat_tree_list), NULL);//note on fail the clearance will be handled inside new_tree_node							

			// flat_tree_list = append_new_flat_tree_node(flat_tree_list, tree_node);
			// printf("$%d pipe / op\n", i);
			i++;	
		}
	//lable EMPETY BLOCK 
		else if (token[i].type == PAREN_CLOSE)
		{
			// printf("empety block %d \n", i);
			tree_node = new_tree_node(BLOCK);//note init the tree node
			if (!tree_node)
				return (free_flat_tree(flat_tree_list), NULL);//note on fail the clearance will be handled inside new_tree_node
			
			tree_node->empty = -1;
			i++;
		}
	//lable FULL BLOCK 
		else
		{
			if (token[i].is_listed == 1)
			{
				i+=2;
				continue;
			}
			// printf("full block %d \n", i);
			tree_node = new_tree_node(BLOCK);//note init the tree node
			// printf("new_block\n");
			while (token[i].data && token[i].type != PIPE && token[i].type != OP_AND && token[i].type != OP_OR && token[i].type != PAREN_CLOSE)//warning maybe this is wrong
			{
				//LABLE save redirection before the cmd 
				if ((token[i].type == RED_INPUT ||
					token[i].type == HER_DOC ||
					token[i].type == RED_APPEND ||
					token[i].type == RED_TRUNK) &&
					token[i].is_listed == 0)
				{
					token[i].is_listed = 1;
					if (!new_red(tree_node, token[i].type, token[i + 1].data))
						return (free_flat_tree(flat_tree_list), NULL);
				}
				
				//LABLE save a word or append it to the list of commands
				else if (token[i].type == WORD)
				{
					tree_node->data = append_command(tree_node->data, token[i].data);
					if (!tree_node->data)
						return (free_flat_tree(flat_tree_list), NULL);
					
					// printf("$%d cmd %s\n",i,  token[i].data);
				}
				
				//LABLE save the redirections ater of () "after"
				else if (token[i].type == PAREN_OPEN)
				{
					//note if came across  '(' and after some commands break the while to make a new node
					tree_node->empty = 1;
					// printf("$%d ()\n", i);
					if (!parenths_redirections(tree_node,  &token[i]))
						return (free_flat_tree(flat_tree_list), NULL);
					i++; //note bcs next line is break u dummass
					break ;
				}
				i++;
			}//while
			
		}//else
		flat_tree_list = append_new_flat_tree_node(flat_tree_list, tree_node);
	}//while
	return (flat_tree_list);
}