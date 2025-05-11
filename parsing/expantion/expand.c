#include "../../include/parsing.h"



char **linked_list_to_double_array(t_node *list)
{
	char **commands_array;
	t_node *tmp;
	int count;
	int		i;

	count = 0;
	tmp = list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	commands_array = malloc(sizeof(char *) * (count + 1));
	if (!commands_array)
		return (NULL);
	tmp = list;
	i = 0;
	while (tmp)
	{
		commands_array[i] = trim_quotes(tmp->data);
		i++;
		tmp = tmp->next;
	}
	// printf("count : %d\n", count + 1);
	commands_array[count] = NULL;
	return (commands_array);
}

int  expand_commands(t_env *env, char ***arr)
{
	int i;
	int	j;
	t_node *list = NULL;
	char **tmp;

	i = -1;
	while ((*arr)[++i])
		(*arr)[i] = do_expantion(env, (*arr)[i]);

	char *joined_arr = join_arr_elements(*arr);
	list = split2(joined_arr);
	if (!list)
		return (free(joined_arr), 0);
	tmp = *arr;
	*arr = linked_list_to_double_array(list);	
	if (!*arr)
		return (0);
	return (1);
}


int expand(t_env *env, t_tree *root)
{
	int state;

	state = 1;
	if (root->data)
		state = expand_commands(env, &root->data);
	if (state && root->redirections)
		state = expand_redirections(env, root->redirections);
	return (state);
}
