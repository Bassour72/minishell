#include "../../include/parsing.h"

t_flat_tree *flat_tree_last(t_flat_tree *flat_tree)
{
	while (flat_tree->next)
		flat_tree = flat_tree->next;
	return (flat_tree);
}

static int operators(t_tree **tree_node, t_token *token, int *i)
{
	*tree_node = new_tree_node(token[*i].type);
	if (!*tree_node)
		return (R_FAIL);//note on fail the clearance will be handled inside new_tree_node							
	(*i)++;
	// printf("inside :%s\n", typetostring[(*tree_node)->type]);
	return (R_SUCCESS);
}

static int close_parenths_block(t_tree **tree_node, int *i)
{

	// printf("empety block %d \n", i);
	*tree_node = new_tree_node(BLOCK);//note init the tree node
	if (!*tree_node)
		return (0);//note on fail the clearance will be handled inside new_tree_node
	
	(*tree_node)->empty = -1;
	(*i)++;
	return (1);
}
static int skip_listed_tokens(t_token *token, int *i)
{
	if (token[*i].is_listed == 1)
	{
		(*i)+=2;
		return (1);
	}
	return (0);
}

static int collect_redirections(t_token *token, int i, t_tree *tree_node)
{
	if ((token[i].type == RED_INPUT ||
		token[i].type == HER_DOC ||
		token[i].type == RED_APPEND ||
		token[i].type == RED_TRUNK) &&
		token[i].is_listed == 0)
	{
		token[i].is_listed = 1;
		if (!new_red(tree_node, token[i].type, token[i + 1].data))
			return (0);
	}
	return (1);
}

static int collect_words(t_token *token, int i, t_tree *tree_node)
{
	if (token[i].type == WORD)
	{
		tree_node->data = append_command(tree_node->data, token[i].data);
		if (!tree_node->data)
			return (0);
		
		// printf("$%d cmd %s\n",i,  token[i].data);
	}
	return (1);
}

int collect_reds_and_cmds(t_token *token, int *i, t_tree *tree_node, t_flat_tree *flat_tree_list)
{

	while (token[*i].data && token[*i].type != PIPE && token[*i].type != OP_AND && token[*i].type != OP_OR && token[*i].type != PAREN_CLOSE)//warning maybe this is wrong
	{
		// printf("done %d\n", *i);
		//LABLE save the redirections ater of () "after"
		//LABLE save redirection before the cmd 

		if (token[*i].type == WORD)
		{
		//LABLE save a word or append it to the list of commands
			if (!collect_words(token, *i, tree_node))
			{
				return (free_flat_tree(flat_tree_list), 0);
			}
		}
		else
		{

			if (!collect_redirections(token, *i, tree_node))
				return (free_flat_tree(flat_tree_list), 0);
		}
		(*i)++;
	}

	return (1);
}

static int open_parenths_block(t_token *token, t_tree **tree_node, int *i)
{

	// printf("empety block %d \n", i);
	*tree_node = new_tree_node(BLOCK);//note init the tree node
	if (!*tree_node)
		return (0);//note on fail the clearance will be handled inside new_tree_node

	(*tree_node)->empty = 1;
	if (!parenths_redirections(*tree_node,  &token[*i]))
		return (0);
	(*i)++;
	return (1);
}

t_flat_tree *create_flat_tree(t_token *token)
{
	t_flat_tree *flat_tree_list; //note linked list
	t_tree *tree_node;  //note: tmp var 
	int i;
	int j; //note () reds index

	flat_tree_list = NULL;
	i = 0;
		
	while (token[i].data)
	{
	//LABLE PIPE / OPERATOR
		if ((token[i].type == PIPE || token[i].type == OP_AND || token[i].type == OP_OR))
		{
			if (!operators(&tree_node, token, &i))
				return (free_flat_tree(flat_tree_list), NULL);
		}
	//lable BLOCK 
		else if (token[i].type == PAREN_CLOSE)
		{
			if (!close_parenths_block(&tree_node, &i))
				return (free_flat_tree(flat_tree_list), NULL);
		}
		else if (token[i].type == PAREN_OPEN)
		{
			if (!open_parenths_block(token, &tree_node, &i))
				return (free_flat_tree(flat_tree_list), NULL);
		}
		else
		{
			if (skip_listed_tokens(token, &i))
					continue ;
			//note init the tree node
			if (!(tree_node = new_tree_node(BLOCK)))
				return (free_flat_tree(flat_tree_list), NULL);
			if (!collect_reds_and_cmds(token, &i, tree_node, flat_tree_list))
				return (NULL);
		}
		// printf("node type %s -> data: \n", typetostring[tree_node->type]);
		flat_tree_list = append_new_flat_tree_node(flat_tree_list, tree_node);
	}
	return (flat_tree_list);
}
