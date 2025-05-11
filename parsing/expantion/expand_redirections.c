#include "../../include/parsing.h"

static void	free_list_and_tmp(t_node *list, char *tmp[2])
{
	t_node *node;

	if (list->next)
	{
		while (list)
		{
			node = list;
			list = list->next;
			free(node->data);
			free(node);
		}
	}
	else
		free(list);
	
	free(tmp[0]);
	free(tmp[1]);
}

static int	do_expand_and_trim_quotes(t_env *env, char *tmp[2], char *data)
{
	tmp[0] = do_expantion(env, data);
	if (!tmp[0])
		return (0);
	tmp[1] = trim_quotes(tmp[0]);
	if (!tmp[1])
		return (free(tmp[0]), 0);
	return (1);
}

int	expand_redirections(t_env *env, t_red *red)
{
	t_node	*list;
	char	*tmp[2];

	while (red)
	{
		if (red->type != HER_DOC)
		{
			if (!do_expand_and_trim_quotes(env, tmp, red->data))
				return (0);
			list = split(tmp[1]);
			if (!list)
				return (free(tmp[0]), free(tmp[1]), 0);
			if (list->next != NULL)//note means nodes are more then one so ambiguous
			{
				free(red->data);
				red->data = NULL;
			} 
			else
				red->data = list->data;
			free_list_and_tmp(list, tmp);
		}	
		red = red->next;
	}
	return (1);
}
