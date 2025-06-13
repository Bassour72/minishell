#include "../../include/parsing.h"

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

int	expand_list_to_array(char ***commands_array, t_expand_node *list)
{
	t_expand_node	*tmp;
	int		count;
	int		i;

	count = get_list_size(list);
	*commands_array = malloc(sizeof(char *) * (count + 1));
	if (!*commands_array)
		return (perror("error: "), R_FAIL);
	tmp = list;
	i = 0;
	while (tmp)
	{
		(*commands_array)[i] = ft_strdup(tmp->data);
		if (!(*commands_array)[i])
		{
			clear_failed_arr(*commands_array, i);
			*commands_array = NULL;
			return (R_FAIL);
		}
		i++;
		tmp = tmp->next;
	}
	(*commands_array)[count] = NULL;
	return (R_SUCCESS);
}