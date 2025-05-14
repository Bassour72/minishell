#include "../../include/parsing.h"

void free_2d_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

char *extract_target_key(char *str)
{
	char *key;
	int len;

	if (*str == '$')
		len = 1;
	else
	{
		len = 0;
		while (str[len] && str[len] != '=' && str[len] != ' ' && str[len] != '\"' && str[len] != '\'' && str[len] != '$' && !is_special(str + len))
			len++;
	}
	key = ft_substr(str, 0, len);
	return (key);
}

void skip_single_quote_area(char *str, int *i)
{
	// printf("skipping single quotes from %d", *i);
	if (str[*i] && str[(*i)] == '\'')
	{
		(*i)++;
		while (str[*i] && str[(*i)++] != '\'')
			(*i)++;
	}
	// printf(" to %d\n", *i);
}

void update_quote(char str_i, int *quote)
{
	if (str_i == '\"' && *quote == 0)
		*quote = 2;
	else if (str_i == '\"' && *quote == 2)
		*quote = 0;
	else if (str_i == '\'' && *quote == 0)
		*quote = 1;
	else if (str_i == '\'' && *quote == 1)
		*quote = 0;
}

char	*join_arr_elements(char **arr)
{
	int i;
	char *joined_arr = NULL;
	char *tmp;

	i = -1;
	while (arr[++i])
	{
		tmp = joined_arr;
		joined_arr = ft_strjoin(joined_arr, arr[i]);
		free(tmp);
		if (!joined_arr)
			return (perror("minishell: error: "), NULL);

		tmp = joined_arr;
		joined_arr = ft_strjoin(joined_arr, " ");
		free(tmp);
		if (!joined_arr)
			return (perror("minishell: error: "), NULL);
	}
	return (joined_arr);
}
