
#include "../../include/parsing.h"

t_tree *parser(t_tree *tree, char *input)
{
	t_token *tokenized_input;
	t_flat_tree *flat_tree;

	if (!validate_quotes(input))
		return (free(input), NULL);
 
	if (!tokenizer(&tokenized_input, input) == R_FAIL)
		return (NULL);

	free_tokens_list(tokenized_input);
	return NULL;

	// print_tokenized_inputs(tokenized_input);
	// return NULL;
	// free(input);
	if (!tokenized_input)
	{
		printf("tokenizer returns NULL\n");
		return (NULL);
	}
	// if (!validate_sytax(tokenized_input))
	// {

	// 	return (free_tokens_list(tokenized_input), NULL);
	// }

	// printf("validate syntax OK\n");

	// print_tokenized_inputs(tokenized_input);

	// return NULL;
	flat_tree = create_flat_tree(tokenized_input);
	if (!flat_tree)
		return (free_tokens_list(tokenized_input), NULL);
	// print_flat_tree(flat_tree);

	free_tokens_list(tokenized_input);

	// return NULL;

	t_tree *root = init_tree(flat_tree);
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
	
	return (root);
}
