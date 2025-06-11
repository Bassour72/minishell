
#include "../../include/parsing.h"

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
	if (!flat_tree)
		return (free_tokens_list(tokenized_input), R_FAIL);
	// print_flat_tree(flat_tree);
	printf("##################################\n");
	free_tokens_list(tokenized_input);
	
	return R_SUCCESS;
	free_tokens_list(tokenized_input);

	// return NULL;

	*tree = init_tree(flat_tree);
	// free(r);

	// // printf("done %s\n", typetostring[root->type]);
	// free_tree(root);
	t_flat_tree *t;
	int k = 1;
	t_tree *pp;
	while (flat_tree)
	{
		t = flat_tree;
		flat_tree = flat_tree->backup_next;
		// // free_tree_node(t->tree_node);
		if (t->tree_node->empty == -1)
			free(t->tree_node);
		// else
		// 	pp = NULL;
		// printf("out %p\n", t);
		free(t);
		// free(pp);
	}


	// expand(env, root);
	// expand_variables(root, env);
	// print_tree(root, 0);

	// return NULL;
	
	return (R_SUCCESS);
}
