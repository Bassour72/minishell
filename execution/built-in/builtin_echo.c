#include "../../include/execution.h"

int has_no_newline_option(t_tree *root, int *value)
{
	int i;
	int j;

	i = 1;
	while (root->data[i] != 0)
	{
		if (root->data[i][0] != '-')
			break;
		j = 1;
		while (root->data[i][j] == 'n')
			j++;
		if (root->data[i][j] == '\0' && j > 1)
			i++;
		else
			break;
	}
	*value = i;
	return (i > 1);
}

int	builtin_echo(t_tree *root)
{
	int i;
	int has_newline;

	i = 1;
	has_newline = 1;
	if (root == NULL || root->data == NULL || root->data[0] == NULL)
		return (1);
	if (has_no_newline_option(root, &i))
		has_newline = 0;
	while (root->data[i] != NULL)
	{
		write(1, root->data[i], ft_strlen(root->data[i]));
		if (root->data[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	if (has_newline)
		write(1, "\n", 1);
	return (0);
}


