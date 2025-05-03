
#include "../include/parsing.h"


// static int look_for_this_type(t_tree *flat_tree,int type, int size, int side)
// {
// 	int	i;

// 	i = size - 1;
// 	if (size == -1)
// 	{
// 		while (i >= 0)
// 		{
// 			if ((flat_tree + i)->type == type && (flat_tree + i)->id == -1)
// 			{
// 				// printf("%d <%s>\n", i, (flat_tree + i)->data[0]);
// 				return (i);	
// 			}
// 			i--;
// 		}
// 	}
// 	else if (side == 1)
// 	{
// 		while ((flat_tree + i)->data)
// 		{
// 			if ((flat_tree + i)->type == type && (flat_tree + i)->id == -1)
// 			{
// 				// printf("%d <%s>\n", i, (flat_tree + i)->data[0]);
// 				return (i);	
// 			}
// 			i++;
// 		}
// 	}
// 	return (-1);//not found
// }
/*
int get_next_root(t_tree *flat_tree, int len, int side)
{
	// int	size;
	int i;

	//get tree size
	// size = 0;
	// while ((flat_tree + size)->data)
	// 	size++;
	//look for opperators
	if ( (i = look_for_this_type(flat_tree, T_OPPERATOR, len, side)) != -1)
		return (i);
	
	// printf("not opperatot\n");
	//look for pipes
	else if ((i = look_for_this_type(flat_tree, PIPE, len, side)) != -1)
		return (i);
	
	//look for redirections
	else if ((i = look_for_this_type(flat_tree, RED_INPUT, len, side)) != -1 || 
			 (i = look_for_this_type(flat_tree, HER_DOC, len, side)) != -1 || 
			 (i = look_for_this_type(flat_tree, RED_TRUNK, len, side)) != -1 || 
			 (i = look_for_this_type(flat_tree, RED_APPEND, len, side)) != -1 )
		return (i);
	
	
	// printf("only command are left\n");	
	return -1;
}
		
char **init_node_data(t_token *tokens, int size)
{
	char	**data_list;
	int		i;
	int		j;

	//if the token is a pipe or an opperator we don't need data or red list
	if (tokens->type == PIPE || tokens->type == T_OPPERATOR)
		return NULL;
	data_list = malloc(sizeof(char *) * (size + 1));
	if (!data_list)
		return (NULL);
	i = 0;
	j = 0;
	// printf("|%s|\n", tokens->data );
	while ((tokens + i)->data && ((tokens + i)->type == WORD || 
			(tokens + i)->type == T_FILE_NAME || 
			(tokens + i)->type == RED_INPUT || 
			(tokens + i)->type == HER_DOC || 
			(tokens + i)->type == RED_APPEND ||
			(tokens + i)->type == RED_TRUNK)) //
	{
		// printf("%d -> [%s]\n",i, (tokens + i)->data);
		if  ((tokens + i)->type == WORD)
		{
			*(data_list + j++) = ft_strdup((tokens + i)->data); //protect
			// printf(">>>>>%s\n", (tokens + i)->data);
		}
		i++;
	}
	printf("j: %d, size: %d\n", j, size);
	*(data_list + j) = NULL;
	return (data_list);
}

int count_tree_nodes(t_token *tokens) //NOTE : maybe unused
{
	int	i;
	int count = 0;

	i = 0;
	while ((tokens + i)->data != NULL)
	{
		if ((tokens + i)->type == WORD)
		{
			while ((tokens + i)->type == WORD)
				i++;
		}
		else
			i++;
		
		count++;
	}
	return (count);
}

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

//count the size for the command and its args
int	count_commad_count_size(t_token *token, int *index)
{
	int	i;
	int size;

	i = 0;
	size = 0;
	while ((token + i)->data && 
		(token + i)->type == WORD || 
		(token + i)->type == T_FILE_NAME ||
		(token + i)->type == RED_INPUT || 
		(token + i)->type == HER_DOC || 
		(token + i)->type == RED_APPEND || 
		(token + i)->type == RED_TRUNK)
	{
		if ((token + i)->type == WORD)
			size++;
		i++;
	}
	//skip the computed tokens
	// printf("ccn i %d", i);
	*index += i;
	return (size);
}
*/
/*

t_red	*init_node_redirections(t_token *token, int start_i)
{
	t_red *redirectins;
	int size;
	int i;

	int right = 0;
	int left = 0;
	size = 0;
	//check right size
	i = 0;
	while ((token + i)->data && (is_red(token + i) || (token + i)->type == WORD))
	{
		if (is_red(token + i))
			right++;
		i++;
	}
	
	//check left size
	i = 1;
	while (start_i - i >= 0 && (is_red(token - i) || (token - i)->type == WORD))
	{
		if (is_red(token - i))
			left++;
		i++;
	}

	size = right + left;
	printf("redirections  number = %d\n", size /2);
	printf("redirections on the right = %d\n", right / 2);
	printf("redirections on the left = %d\n", left / 2);

	//note : create redirections
	redirectins = malloc(sizeof(t_red) * ((size / 2) + 1));
	int j = 0;

	for(int i = 0; i < right; i += 2)//todo transform to while
	{
		// printf("%s ", (token + i + 2)->data);
		if ((token + i + 1)->type == RED_INPUT)
		{
			(redirectins + j)->type = IN_RED; 
			(redirectins + j)->data = ft_strdup((token + i + 2)->data);//todo : protect if strdup fails
			// printf(">R>%s\n", (redirectins + j)->data);
		}
		else if ((token + i + 1)->type == RED_TRUNK)
		{
			(redirectins + j)->type = TRUNK;
			(redirectins + j)->data = ft_strdup((token + i + 2)->data);//todo : protect if strdup fails
			// printf(">R>%s\n", (redirectins + j)->data);
		}
		else if ((token + i + 1)->type == RED_APPEND)
		{
			(redirectins + j)->type = APPEND;
			(redirectins + j)->data = ft_strdup((token + i + 2)->data);//todo : protect if strdup fails	
			// printf(">R>%s\n", (redirectins + j)->data);
		}
		else if ((token + i + 1)->type == HER_DOC)
		{
			(redirectins + j)->type = HER_DOC;
			(redirectins + j)->data = ft_strdup((token + i + 2)->data);//todo : protect if strdup fails	
			// printf(">R>%s\n", (redirectins + j)->data);
		}
			// printf(">L>%s\n", (redirectins + j)->data);
		// i -= 2;
		j++;
	}
	// printf("<<\n");

	for(int i = 0; i < left; i += 2)
	{
		// printf("%s ", (token - i - 1)->data);
		if ((token - i - 2)->type == RED_INPUT)
		{
			(redirectins + j)->type = IN_RED; 
			(redirectins + j)->data = ft_strdup((token - i - 1)->data);//todo : protect if strdup fails
			// printf(">R>%s\n", (redirectins + j)->data);
		}
		else if ((token - i - 2)->type == RED_TRUNK)
		{
			(redirectins + j)->type = TRUNK;
			(redirectins + j)->data = ft_strdup((token - i - 1)->data);//todo : protect if strdup fails
			// printf(">R>%s\n", (redirectins + j)->data);
		}
		else if ((token - i - 2)->type == RED_APPEND)
		{
			(redirectins + j)->type = APPEND;
			(redirectins + j)->data = ft_strdup((token - i - 1)->data);//todo : protect if strdup fails	
			// printf(">R>%s\n", (redirectins + j)->data);
		}
		else if ((token - i - 2)->type == HER_DOC)
		{
			(redirectins + j)->type = HER_DOC;
			(redirectins + j)->data = ft_strdup((token - i - 1)->data);//todo : protect if strdup fails	
			// printf(">L>%s\n", (redirectins + j)->data);
		}
	}
	
	(redirectins + size / 2)->data = NULL;
	return (redirectins);
}
*/

// void new_flat_tree_node(t_flat_tree *list, t_tree *tree_node)
// {
// 	if (list == NULL)
// 	{
		
// 	}
// }

// t_flat_tree *create_flat_tree(t_token *tokens)
// {
// 	int	i;
// 	t_flat_tree *flat_tree;
	
// 	i = 0;
// 	while ((tokens + i)->data)
// 	{
		
// 	}	
// }
/*
t_tree *create_flat_tree(t_token *tokens)
{
	t_tree 	*tree_nodes_list;
	int 	list_size;
	int 	i;
	int 	j;
	int		tmp;
	
	tree_nodes_list = malloc(sizeof(t_tree) * (10 + 1));//protect more	
	j = 0;
	i = 0;
	//fixme : this wont work if the input is just a redirection
	while ((tokens + i)->data)
	{
		list_size = 0;
		if ((tokens + i)->type == WORD)
		{
			tmp = i;
			// //calculate command list size and skipp reds and file names
			list_size =  count_commad_count_size(tokens + i, &tmp);
			// printf("i: |%d|\nsize : |%d|\n", tmp , list_size);
			// printf("list_size : |%s|\n", (tokens + i)->data);
			// printf("list_size: %d\n", list_size);
			(tree_nodes_list + j)->data = init_node_data(tokens + i , list_size );
			(tree_nodes_list + j)->redirections = init_node_redirections(tokens + i, i);
			// (tree_nodes_list + j)->redirections = init_redirections(tokens + i, list_size)
			(tree_nodes_list + j)->type = WORD;
			// printf("tmp: %d\n", tmp);
			i = tmp;
		}
		else if ((tokens + i)->type == PIPE || (tokens + i)->type == T_OPPERATOR)
		{
			printf("here\n");
			// printf("%d -> [%s]\n", i, tokens[i].data);
			(tree_nodes_list + j)->data = init_node_data(tokens + i , 1 );
			(tree_nodes_list + j)->type = (tokens + i)->type;
			i++;
		}
		else
			i++;
		// printf("[i: %d, j: %d]\n", i, j);
		// (tree_nodes_list + j)->id = -1; // set -1 to all tree nodes id as a default value
		(tree_nodes_list + j)->left = (tree_nodes_list + j)->right = NULL;
		
		//lable: debuging 
		if (tree_nodes_list[j].type == WORD)
		{
			//note print redirections
			// printf("redirections: ");
			// if (tree_nodes_list[j].redirections == NULL )
			// 	printf("none\n");
			// else
			// {
			// 	for( int k = 0; tree_nodes_list[j].redirections[k].data ; k++)
			// 	{
			// 		printf("{%s }", tree_nodes_list[j].redirections[k].data);
			// 	}
			// 	printf("\n");
			// }
			//note print command list
			printf("CMD: ");
			printf("{");
			for (int i = 0; tree_nodes_list[j].data[i]; i++)
			{
				printf("[%s] ", tree_nodes_list[j].data[i]);
			}
			printf("}\n");
		}
		else if ((tree_nodes_list + j)->type  == PIPE)
			printf("PIPE: \n");
		else if ((tree_nodes_list + j)->type  == T_OPPERATOR)
			printf("OPERATOR: \n");
		//done printing
		j++;
	}
	
	(tree_nodes_list + j)->data = NULL;
	return (tree_nodes_list);
}
*/