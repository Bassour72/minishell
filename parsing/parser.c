
#include "../include/parsing.h"

//LABLE functions 

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

static char *typetostring[] = {
	[OP_AND] = "AND",
	[OP_OR] = "OR",
	[PIPE] = "PIPE",
	[BLOCK] = "BLOCK"
};

static void print_tokenized_inputs(t_token *tokenized_input)
{
	int i = -1;
	while((tokenized_input + ++i)->data)
	{
		if ((tokenized_input + i)->type == PIPE)
			printf("->>PIPE :");
		else if ((tokenized_input + i)->type == WORD)
			printf("->>CMD  :");
		else if ((tokenized_input + i)->type == T_FILE_NAME)
			printf("->>FILE_NAME :");
		else if ((tokenized_input + i)->type == RED_INPUT)
			printf("->>IN_REDR :");
		else if ((tokenized_input + i)->type == RED_APPEND)
			printf("->>APND_REDR :");		else if ((tokenized_input + i)->type == RED_TRUNK)
			printf("->>TRNK_REDR :");
		else if ((tokenized_input + i)->type == HER_DOC)
			printf("->>HER_DOC :");
		else if ((tokenized_input + i)->type == OP_AND)
			printf("->>OPER AND :");
		else if ((tokenized_input + i)->type == OP_OR)
			printf("->>OPER OR :");
		else if ((tokenized_input + i)->type == PAREN_OPEN)
			printf("->>PAREN open :");
		else if ((tokenized_input + i)->type == PAREN_CLOSE)
			printf("->>PAREN close :");
		printf(" %s\n", tokenized_input[i].data);
	}
}
void print_flat_tree(t_tree *list)
{
	int j = 0;
	int i = 0;

	while (list[j].data)
	{
	
		// printf("id (%d) -> { ", list[j].id);
		i = 0;
		while (list[j].data[i])
		{
			printf("[%s] ", list[j].data[i]);
			i++;
		}
		printf("}\n");
		j++;
	}
}

void print_tree(t_tree *root, int i)
{
	if (!root)
		return ;
	for(int j = 0; j < i; j++)
		printf("\t");
	printf("%s", typetostring[root->type]);
	if (root->type == BLOCK)
	{		
		printf(" CMD=>");
		if (root->data)
		{
			for(int i = 0; root->data[i]; i++)
				printf("[%s] ", root->data[i]);
			printf("");
		}
		else
			printf(" NULL");

		
		printf(" | RED=> ");
		if (root->redirections)
		{
			t_red *r = root->redirections;
			while (r)
			{
				printf("[%s] ", r->data);
				r = r->next;
			}
		}
		else
			printf(" NULL");
	}
	printf("\n");
	print_tree(root->right, i + 1);
	print_tree(root->left, i + 1);
}

t_tree *new_tree_node(t_type type)
{
	t_tree *tree_node;

	tree_node = malloc(sizeof(t_tree));//todo protect
	if (!tree_node)
		return (NULL);
	tree_node->data = NULL;//todo naming
	tree_node->file_name = NULL;//check usage 
	tree_node->redirections = NULL;
	tree_node->type = type;
	tree_node->left = tree_node->right = NULL;
	tree_node->empty = 0;
	return (tree_node);
}


void free_tree_node(t_tree *node)
{
	int		i;
	t_red	*red;
	t_red	*tmp;

	if (node->data)
	{
		i = -1;
		while (node->data[++i])
			free(node->data[i]);
		free(node->data);
	}
	if (node->redirections)
	{
		red = node->redirections;
		while (red)
		{
			tmp = red;
			red = red->next;
			free(tmp->data);
			free(tmp);
		}
	}
	free(node);
}

void free_flat_tree_list(t_flat_tree *flat_list)
{
	t_flat_tree	*tmp;

	while (flat_list)
	{
		tmp = flat_list;
		flat_list = flat_list->next;
		free_tree_node(tmp->tree_node);
		free(tmp);
	}
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


int new_red(t_tree  *tree_node, t_type type, char *data)
{
	t_red	*new_red;
	t_red	*last_red;

	new_red = malloc(sizeof(t_red));
	if (!new_red)
	{
		free_tree_node(tree_node);
		return (0);
	}
	
	new_red->data = ft_strdup(data);
	if (!new_red->data)
	{
		free(new_red);
		free_tree_node(tree_node);
		return (0);
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

			if (!new_red(tree_node, (token + i)->type, token[i + 1].data))
				return (0);
		}
		else //note if the token after the ) is not a red break the loop
			break;
		i += 2;
	}
	return (1);
}

void	free_flat_tree(t_flat_tree *flat_tree)
{
	t_flat_tree *tmp;

	while (flat_tree)
	{
		tmp = flat_tree;
		flat_tree = flat_tree->next;
		free_tree_node(tmp->tree_node);
		free(tmp);
	}
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
			printf("empety block %d \n", i);
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



t_flat_tree *flat_tree_last(t_flat_tree *flat_tree)
{
	while (flat_tree->next)
		flat_tree = flat_tree->next;
	return (flat_tree);
}


t_tree *init_tree(t_flat_tree *ft)
{
	t_flat_tree *flat;
	t_flat_tree *right;
	t_flat_tree *left;
	int p = 0;

	flat =  flat_tree_last(ft);
	while (flat->prev)
	{
		if (flat->tree_node->empty == -1)
			p++;
		else if (flat->tree_node->empty == 1)
			p--;
		if (p == 0 && (flat->tree_node->type == OP_AND || flat->tree_node->type == OP_OR))
		{
			right = flat->next;
			left = flat->prev;
			flat->next->prev = NULL;
			flat->prev->next = NULL;
			flat->next = NULL;
			flat->prev = NULL;
			flat->tree_node->right = init_tree(right);
			flat->tree_node->left = init_tree(left);
			return (flat->tree_node);
		}
		flat = flat->prev;
	}

	p = 0;
	flat =  flat_tree_last(ft);
	while (flat->prev)
	{
		if (flat->tree_node->empty == -1)
			p++;
		else if (flat->tree_node->empty == 1)
			p--;
			
		if (p == 0 && flat->tree_node->type == PIPE)
		{
			right = flat->next;
			left = flat->prev;
			flat->next->prev = NULL;
			flat->prev->next = NULL;
			flat->next = NULL;
			flat->prev = NULL;
			flat->tree_node->right = init_tree(right);
			flat->tree_node->left = init_tree(left);
			return (flat->tree_node);
		}
		flat = flat->prev;
	}

	flat = flat_tree_last(ft);
	if (flat->tree_node->empty == -1)
	{
		flat->prev->next = NULL;
		// flat->prev = NULL; // maybe this is unessessiry
		while (flat->prev)
		{
			flat = flat->prev;
			// printf("he\n");
		}

		flat->next->prev = NULL;
		// flat->prev = NULL; // maybe this is unessessiry
		right = flat->next;
		flat->tree_node->right = init_tree(right);
		return (flat->tree_node);
	}
	return (ft->tree_node);
	return NULL;
}

void free_tree(t_tree *root)
{
	if (root->right)
		free_tree(root->right);
	if (root->left)
		free_tree(root->left);
	free_tree_node(root);
}


t_tree *parser(t_tree *tree, char *input)
{
	t_token *tokenized_input;
	t_flat_tree *flat_tree;

	if (!validate_quotes(input))
		return (NULL);
	tokenized_input = tokenizer( input);
	if (!tokenized_input)
	{
		printf("tokenizer returns NULL\n");
		return (NULL);
	}
	if (!validate_sytax(tokenized_input))
		return (free_tokens_list(tokenized_input), NULL);

	// print_tokenized_inputs(tokenized_input);

	// return NULL;
	flat_tree = create_flat_tree(tokenized_input);
	if (!flat_tree)
		return (free_tokens_list(tokenized_input), NULL);

	free_tokens_list(tokenized_input);

	// return NULL;
	t_tree *root = init_tree(flat_tree);

	// // printf("done %s\n", typetostring[root->type]);
	// free_tree(root);
	t_flat_tree *t;
	while (flat_tree)
	{
		t = flat_tree;
		flat_tree = flat_tree->next;
		// free_tree_node(t->tree_node);
		free(t);
	}


	// expand(env, root);
	// expand_variables(root, env);
	print_tree(root, 0);

	// return NULL;
	
	return (root);
}
