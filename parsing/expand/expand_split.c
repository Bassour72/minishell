#include "../../include/parsing.h"

static size_t	count_strings(char *str, char c)
{
	size_t	i;

	i = 0;
	while (*str != '\0')
	{
		if (*str != c)
		{
			while (*str != c && *str)
				str++;
			i++;
		}
		else
			str++;
	}
	return (i);
}

static char	*custom_strdub(char *s, size_t len)
{
	char	*res;

	res = (char *)malloc((len + 1) * sizeof(char));
	if (res == 0)
		return (NULL);
	ft_strlcpy(res, s, len + 1);
	return (res);
}

static void free_store(t_expand_node *store)
{
	t_expand_node *tmp;

	while (store)
	{
		tmp = store;
		store = store->next;
		free(tmp->data);
		free(tmp);
	}
}

static int append_node_expand(t_expand_node **store, char *data)
{
	t_expand_node *new_node;
	t_expand_node *tmp;

	new_node = malloc(sizeof(t_expand_node));
	if (!new_node)
		return (perror("error: "), R_FAIL);
	new_node->data = data;
	new_node->joinable = 0;
	new_node->next = NULL;
	if (!*store)
		*store = new_node;
	else
	{
		tmp = *store;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (R_SUCCESS);
}

static t_expand_node *do_split(char *str, char split_char)
{
	int				start;
	t_expand_node	*store;
	char 			*data;
	int				i;

	store = NULL;

	i = 0;
	while (str[i])
	{
		while (str[i] == split_char)
			i++;
		start = i;
		while (str[i] && str[i] != split_char)
			i++;
		data = custom_strdub(str + start, i - start);
		if (data == NULL)
			return (free_store(store), NULL);
        if (append_node_expand(&store, data) == R_FAIL)
			return (free(store), NULL);
	}
	return (store);
}

t_expand_node	*expand_split(char const *str, char c, int joinable)
{
	t_expand_node	*store = NULL;
	t_expand_node	*tmp;

	store = NULL;
	store = do_split((char *)str, c);
	if (store && joinable)
	{
		tmp = store;
		while (tmp->next)
			tmp = tmp->next;
		tmp->joinable = joinable;
	}
	return (store);
}
