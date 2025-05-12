#include "../../include/parsing.h"


const char *typetostring[] = {
    [OP_AND] = "AND",
    [OP_OR] = "OR",
    [PIPE] = "PIPE",
    [BLOCK] = "BLOCK"
};

void print_tokenized_inputs(t_token *tokenized_input)
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