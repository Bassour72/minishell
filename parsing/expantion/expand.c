#include "../../include/parsing.h"

void clear_failed_arr(char **arr, int fail_point)
{
	while (--fail_point >= 0)
		free(arr[fail_point]);
	free(arr);
}

char **linked_list_to_double_array(t_node *list)
{
	char	**commands_array;
	t_node	*tmp;
	int		count;
	int		i;

	count = get_list_size(list);
	commands_array = malloc(sizeof(char *) * (count + 1));
	if (!commands_array)
		return (perror("minishell: error: "), NULL);
	tmp = list;
	i = 0;
	while (tmp)
	{
		commands_array[i] = trim_quotes(tmp->data);
		if (!commands_array[i])
		{
			clear_failed_arr(commands_array, i);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	commands_array[count] = NULL;
	return (commands_array);
}

int  expand_commands(t_env *env, char ***arr)
{
	int i;
	int	j;
	t_node *list = NULL;
	char **tmp;
	char *old_str;

	i = -1;
	while ((*arr)[++i])
		(*arr)[i] = do_expantion(env, (*arr)[i]);

	char *joined_arr = join_arr_elements(*arr);
	list = split(joined_arr);

	free(joined_arr);
	// printf("joined_arr [%s]\n", joined_arr);
	if (!list)
		return (0);
	tmp = *arr;
	*arr = linked_list_to_double_array(list);
	

	for (int i = 0; tmp[i]; i++)
		free(tmp[i]);
	free(tmp);
	t_node *tmp_list;
	while (list)
	{
		tmp_list = list;
		list = list->next;
		free(tmp_list);
	}

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
