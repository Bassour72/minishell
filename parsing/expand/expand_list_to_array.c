#include "../../include/expand.h"

static void clear_failed_arr(char **arr, int fail_point)
{
	while (--fail_point >= 0)
		free(arr[fail_point]);
	free(arr);
}

static int get_list_size(t_expand_node *list)
{
	int	size;

	size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}

char **expand_list_to_array(t_expand_node *list)
{
	char	**commands_array;
	t_expand_node	*tmp;
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
		commands_array[i] = ft_strdup(tmp->data);
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