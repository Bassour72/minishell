
#include "../../include/parsing.h"

void clean_the_flat_tree(t_flat_tree *flat_tree)
{
	t_flat_tree *tmp;

	while (flat_tree)
	{
		tmp = flat_tree;
		flat_tree = flat_tree->backup_next;
	
		if (tmp->tree_node->empty == -1)
			free(tmp->tree_node);

		free(tmp);
	}
}

int	parser(t_tree **tree, char *input)
{
	t_token *tokenized_input = NULL;
	t_flat_tree *flat_tree;

	if (!validate_quotes(input))
		return (free(input), R_SUCCESS);//check this 
 
	if (tokenizer(&tokenized_input, input) == R_FAIL)
		return (free_tokens_list(tokenized_input), R_FAIL);

	
	
	// print_tokenized_inputs(tokenized_input);
	// return NULL;
	// free(input);

	// if (!validate_sytax(tokenized_input))
	// {

	// 	return (free_tokens_list(tokenized_input), NULL);
	// }

	// printf("validate syntax OK\n");

	// print_tokenized_inputs(tokenized_input);

	// return NULL;
	flat_tree = create_flat_tree(tokenized_input);
	free_tokens_list(tokenized_input);
	if (!flat_tree)
		return (R_FAIL);
	// print_flat_tree(flat_tree);
	

	// return NULL;

	*tree = init_tree(flat_tree);
	// free(r);

	// // printf("done %s\n", typetostring[root->type]);
	// free_tree(root);
	clean_the_flat_tree(flat_tree);

	// expand(env, root);
	// expand_variables(root, env);
	// print_tree(root, 0);

	// return NULL;
	
	return (R_SUCCESS);
}
