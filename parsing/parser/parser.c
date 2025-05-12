
#include "../../include/parsing.h"

t_tree *parser(t_tree *tree, char *input)
{
	t_token *tokenized_input;
	t_flat_tree *flat_tree;

	if (!validate_quotes(input))
		return (NULL);
	tokenized_input = tokenizer( input);
	// print_tokenized_inputs(tokenized_input);
	// return NULL;
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
	// print_tree(root, 0);

	// return NULL;
	
	return (root);
}
