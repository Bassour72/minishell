#include "../include/execution.h"

void execution(t_tree *root)
{
	builtin_echo(root);

	if (root == NULL)
		return ;
	execution(root->left);
	printf("%s\n", *root->data);
	execution(root->right);
}