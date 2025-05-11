#include "../../include/parsing.h"

static int inside_quotes(char *str, int *i, char **result, char *type)
{
	int start;
	char *sub_str;
	char *tmp_result;

	*type = str[*i];
	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != *type)
		(*i)++;
	sub_str = ft_substr(str, start, (*i) - start);
	if (!sub_str)
		return (perror("minishell: error: "), free(*result), 0);
	tmp_result = *result;
	*result = ft_strjoin(*result, sub_str);
	(free(tmp_result), free(sub_str));
	if (!*result)
		return (perror("minishell: error: "), free(sub_str), 0);
	(*i)++;
	return (1);
}

static int outside_quotes(char *str, int *i, char **result, char *type)
{
	int start;
	char *sub_str;
	char *tmp_result;

	*type = '\0';
	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '\"')
		(*i)++;
	sub_str = ft_substr(str, start, (*i) - start);
	if (!sub_str)
		return (perror("minishell: error: "), free(*result), 0);
	tmp_result = *result;
	*result = ft_strjoin(*result, sub_str);
	(free(tmp_result), free(sub_str));
	if (!*result)
		return (perror("minishell: error: "), free(tmp_result), 0);
	return (1);
}

char *trim_quotes(char *str)
{
		int		i;
		char	type = '\0';
		char	*result = NULL;

		i = 0;
		while (str[i])
		{
			if (str[i] && (str[i] == '\'' || str[i] == '\"'))
			{
				if (!inside_quotes(str, &i, &result, &type))
					return (NULL);
			}
			else
			{
				if (!outside_quotes(str, &i, &result, &type))
					return (NULL);
			}
		}
	return (result);
}