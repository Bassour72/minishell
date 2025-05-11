#include "../include/execution.h"

#include <string.h>
//todo remove this 
int	is_buit_in_function(char *command)
{
	if (strcmp((command), "echo") == 0 || strcmp((command), "ECHO") == 0)
		return (0);
	if (strcmp((command), "cd") == 0 || strcmp((command), "CD") == 0)
		return (0);
	if (strcmp((command), "env") == 0 || strcmp((command), "env") == 0)
		return (0);
	if (strcmp((command), "exit") == 0 || strcmp((command), "exit") == 0)
		return (0);
	if (strcmp((command), "export") == 0 || strcmp((command), "export") == 0)
		return (0);
	if (strcmp((command), "pwd") == 0 || strcmp((command), "pwd") == 0)
		return (0);
	if (strcmp((command), "unset") == 0 || strcmp((command), "unset") == 0)
		return (0);
	return (1);
}

void execution(t_tree *root)
{
	// builtin_echo(root);

	if (root == NULL)
		return ;
	// execution(root->left);
	// printf("%s\n", *root->data);
	// execution(root->right);
	// if (is_buit_in_function(root->data[0]))
	// 	printf("is not built-in fucntion just command \n");

}