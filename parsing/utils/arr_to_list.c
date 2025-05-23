#include "../include/parsing.h"

void static clear_on_fail(t_node **list)//review  
{
	t_node	*tmp_list;
	t_node	*tmp;
	char	*data;

	tmp_list = *list;
	while (tmp_list)
	{
		tmp = tmp_list;
		tmp_list = tmp_list->next;
		free(tmp->data);
		free(tmp);
	}

}

int arr_to_list(char **arr, t_node **list)
{
	int		i;
	char	*data;

	i = -1;
	while (arr[++i])
	{
		data = ft_strdup(arr[i]);
		if (!data)
			return (clear_on_fail(list), 0);
		if (!append_node(list, ft_strdup(arr[i])))
			return (clear_on_fail(list), 0);
	}
}