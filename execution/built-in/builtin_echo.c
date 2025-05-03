#include "../../include/execution.h"
//TODO remove this 
#include <string.h>


char *builtin_echo(t_tree *root)
{
	int	i;
	if (root == NULL)
		return (NULL);
	i = 1;
	if (root->data && strcmp(root->data[0], "echo")== 0)
	{
		
		while (root->data[i] != NULL)
		{
			printf("[%s]", root->data[i]);
			i++;
		}
		
	}
	return (NULL);
}