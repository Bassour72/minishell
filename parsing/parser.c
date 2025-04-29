/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:03:00 by massrayb          #+#    #+#             */
/*   Updated: 2025/04/28 13:54:34 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

//debug functions 

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
			printf("->>APND_REDR :");
		else if ((tokenized_input + i)->type == RED_TRUNK)
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
	if (root->data)
		printf(" %s", root->data[0]);
	if (root->redirections)
	{
		printf(" : ");
		t_red *r = root->redirections;
		while (r)
		{
			printf("-%s ", r->data);
			r = r->next;
		}
	}
	printf("\n");
	print_tree(root->right, i + 1);
	print_tree(root->left, i + 1);
}
//----------------------------------------------------------------------
//[helper functions] ++++++++++++++++++++++++++++++++++++++++++++++++++++++
int get_flat_tree_size(t_tree *tree)
{
	int size = 0;
	while ((tree + ++size)->data);
	return (size);
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static int is_divisible(t_token *list)
{
	int i;

	i = -1;
	while ((list + ++i)->data)
	{
		if (list->type != WORD)
			return (1);
	}
	return (0);
}


void stage_one(t_tree *tree, int len)
{
	int i;

	i = -1;
	
	(void)len;
	while ((tree + ++i)->data)
	{
		//if the the input start or ends with pipe like this "cat | " a heap-over-flow error will apear
		if ((tree + i)->type == PIPE && (tree + i - 1)->type == WORD && (tree + i + 1)->type == WORD) 
		{
			(tree + i)->left = tree + i - 1; //setup the left side of a pipe to it's left cmd
			(tree + i)->right = tree + i + 1; //setup the right side of a pipe to it's right cmd
		}
	}

	// i = -1;
	// while ((tree + ++i)->data)
	// {
	// 	//if the the input start or ends with pipe like this "cat > " a heap-over-flow error will apear
	// 	if ((tree + i)->type == REDIRECTION && (tree + i - 1)->type == CMD && (tree + i + 1)->type == CMD) 
	// 	{
	// 		(tree + i)->left = tree + i - 1; //setup the left side of a pipe to it's left cmd
	// 		(tree + i)->right = tree + i + 1; //setup the right side of a pipe to it's right cmd
	// 	}
	// }
	
}

t_tree *new_tree_node(t_type type)
{
	t_tree *tree_node;
	// if (type == BLOCK)
	// 	printf("#1\n");

	tree_node = malloc(sizeof(t_tree));//todo protect
	tree_node->data = NULL;//todo naming
	tree_node->file_name = NULL;//check usage 
	tree_node->redirections = NULL;
	tree_node->type = type;
	tree_node->left = tree_node->right = NULL;
	tree_node->empty = 0;
	return (tree_node);
}

//flat_tree_list as a list
//tree_node as a data 
t_flat_tree *append_new_flat_tree_node(t_flat_tree *flat_tree_list, t_tree *tree_node)
{
	t_flat_tree	*new_flat_tree_list;
	t_flat_tree	*tmp;
	
	//note create new flat tree
	if (!flat_tree_list)
	{
		new_flat_tree_list = malloc(sizeof(t_flat_tree));//todo protect
		new_flat_tree_list->tree_node = tree_node;
		new_flat_tree_list->next = NULL;
		new_flat_tree_list->prev = NULL;
		return (new_flat_tree_list);
	}
	
	//note append a new flat tree node to the flat tree list
	tmp = flat_tree_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_flat_tree));//todo protect
	tmp->next->tree_node = tree_node;
	tmp->next->next = NULL;
	tmp->next->prev = tmp;
	// printf("append %p -> %p \n", tmp, tree_node);
	return (flat_tree_list);
}

void	parenths_redirections(t_tree *tree_node, t_token *token)
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

			t_red *new_red = malloc(sizeof(t_red));
			new_red->type = token[i].type;
			new_red->data = ft_strdup(token[i + 1].data); //warning if the next token is not a filename type undefined behaviour will apear
			new_red->next = NULL;
			if (tree_node->redirections == NULL)
			{
				tree_node->redirections = new_red;
			}
			else
			{
				tmp_red = tree_node->redirections;
				while(tmp_red->next)
					tmp_red = tmp_red->next;
				tmp_red->next = new_red;
			}
				// printf("$%d red\n", i);
		}
		else //note if the token after the ) is not a red break the loop
			break;
		i+=2;
	}
}

t_flat_tree *create_flat_tree(t_token *token)
{
	t_flat_tree *flat_tree_list = NULL; //note linked list
	t_flat_tree *tmp; //note traverse the linked list
	t_red	*tmp_red;
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
			// flat_tree_list = append_new_flat_tree_node(flat_tree_list, tree_node);
			// printf("$%d pipe / op\n", i);
			i++;	
		}
	//lable EMPETY BLOCK 
		else if (token[i].type == PAREN_CLOSE)
		{
			// printf("empety block %d \n", i);
			tree_node = new_tree_node(BLOCK);//note init the tree node
			tree_node->empty = -1;
				// printf("%d )))))))))))\n", i);
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
				//LABLE save redirection before the cmd or the ()
				if ((token[i].type == RED_INPUT ||
					token[i].type == HER_DOC ||
					token[i].type == RED_APPEND ||
					token[i].type == RED_TRUNK) &&
					token[i].is_listed == 0)
				{
					token[i].is_listed = 1;

					t_red *new_red = malloc(sizeof(t_red));
					new_red->type = token[i].type;
					new_red->data = ft_strdup(token[i + 1].data); //warning if the next token is not a filename type undefined behaviour will apear
					new_red->next = NULL;
					if (tree_node->redirections == NULL)
					{
						tree_node->redirections = new_red;
					}
					else
					{
						tmp_red = tree_node->redirections;
						while(tmp_red->next)
							tmp_red = tmp_red->next;
						tmp_red->next = new_red;
					}
						// printf("$%d red\n", i);
				}
				
				//LABLE save a word or append it to the list of commands
				else if (token[i].type == WORD)
				{
					tree_node->data = append_command(tree_node->data, token[i].data);
					
					// printf("$%d cmd %s\n",i,  token[i].data);
				}
				
				//LABLE save the redirections ater of () "after"
				else if (token[i].type == PAREN_OPEN)
				{
					//note if came across  '(' and after some commands break the while to make a new node
					tree_node->empty = 1;
					// printf("$%d ()\n", i);
					parenths_redirections(tree_node,  &token[i]);
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

/*
t_tree *init_tree(t_flat_tree *ft)
{
	t_flat_tree *flat;
	t_flat_tree *right;
	t_flat_tree *left;
	
	flat = flat_tree_last(ft);
	while (flat->prev)
	{
		if (flat->tree_node->type == OP_AND || flat->tree_node->type == OP_OR)
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
	while (flat->prev)
	{
		if (flat->tree_node->type == PIPE)
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

	// ft->tree_node->

	return (ft->tree_node);
}
*/

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
			// if (flat->tree_node->empty == -1)
			// 	p++;
			// else if (flat->tree_node->empty == 1)
			// 	p--;
			flat = flat->prev;
			// printf("he\n");
		}
		// printf("%p\n", flat->prev);
		// printf("%s with %d\n", typetostring[flat->tree_node->type], flat->tree_node->empty);
		flat->next->prev = NULL;
		// flat->prev = NULL; // maybe this is unessessiry
		right = flat->next;
		flat->tree_node->right = init_tree(right);
		return (flat->tree_node);
	}

	
	return (ft->tree_node);
	return NULL;
}

t_tree *parser(t_tree *tree, char *input)
{
	t_token *tokenized_input;

	tokenized_input = tokenizer(input);
	print_tokenized_inputs(tokenized_input);

	t_flat_tree *flat_tree = create_flat_tree(tokenized_input);

	t_flat_tree *t = flat_tree;
	int i = 0;
	while (t)
	{
		// if (t->tree_node->type == BLOCK && t->tree_node->empty == 0)
		// {
		// 	printf("%p\n", t->tree_node->data[0]);
		// }
		// printf("%d ---------\n",i);
		if (t->tree_node->type == BLOCK)
		{
			// printf("###########\n");
			if (t->tree_node->empty == 0 && t->tree_node->data)
			{
				printf("B %s | ", t->tree_node->data[0]);
			}
			else if (t->tree_node->empty == -1)
				printf(") ");
			else if (t->tree_node->empty == 1)
				printf("(  ");
			t_red * r = t->tree_node->redirections; 
			while (r)
			{
				printf("-%s ", r->data);
				r = r->next;
			}
			printf("\n");
		}
		i++;
		t = t->next;
	}
		printf("flat tree len : %d\n", i);

	//todo get done with OPs

	t_tree *root = init_tree(flat_tree);
	// printf("done %s\n", typetostring[root->type]);
	print_tree(root, 0);
	return (NULL);
}
