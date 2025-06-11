#include "../../include/parsing.h"

int is_red(t_token *token)
{
	//note : is a redirection yes or no
	if (token->type == T_FILE_NAME || 
		token->type == RED_INPUT ||
		token->type == HER_DOC || 
		token->type == RED_APPEND ||
		token->type == RED_TRUNK)
	{
		return (1);
	}
	return (0);
}

int new_red(t_tree  *tree_node, t_type type, char *data)//checked
{
	t_red	*new_red;
	t_red	*last_red;

	new_red = malloc(sizeof(t_red));
	if (!new_red)
		return (free_tree_node(tree_node), R_FAIL);
	
	new_red->data = ft_strdup(data);
	if (!new_red->data)
	{
		free(new_red);
		free_tree_node(tree_node);
		return (R_FAIL);
	}
	// free(data); //todo do i need to free data after strdup
	new_red->type = type;
	new_red->next = NULL;
	
	if (!tree_node->redirections)
		tree_node->redirections = new_red;
	else
	{
		last_red = tree_node->redirections;
		while (last_red->next)
			last_red = last_red->next;
		last_red->next = new_red;	
	}
	return (1);
}

int	parenths_redirections(t_tree *tree_node, t_token *token)
{
	//lable this function looks for reds if '(' appeared o sf
	t_red	*tmp_red;
	int		p;
	int		i;

	i = 0;
	p = 0;
	while (token[i].data)//note walk tell the )
	{
		if (token[i].type == PAREN_OPEN)
			p++;
		else if (token[i].type == PAREN_CLOSE)
			p--;
		// printf("data = %s, p = %d\n", token[i].data, p);
		if (p == 0)
			break;
		i++;
	}
	i++;
	while (token[i].data)//note collect the reds after ')' symbol
	{
	// printf("i = %d\n", i);
		if ((token[i].type == RED_INPUT ||
			token[i].type == HER_DOC || 
			token[i].type == RED_APPEND ||
			token[i].type == RED_TRUNK) && 
			token[i].is_listed == 0)
		{
			(token + i)->is_listed = 1;

			if  (new_red(tree_node, (token + i)->type, token[i + 1].data) == R_FAIL)
				return (R_FAIL);
		}
		else //note if the token after the ) is not a red break the loop
			break;
		i += 2;
	}
	return (R_SUCCESS);
}
