#include "../../include/parsing.h"

char *extract_target_key(char *str)
{
	char *key;
	int len;

	if (*str == '$')
		len = 1;
	else
	{
		len = 0;
		while (str[len] && str[len] != ' ' && str[len] != '\"' && str[len] != '\'' && str[len] != '$' && !is_special(str + len))
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

char *do_expantion(t_env *env, char *str)
{
	int i;
	char *new_str;
	char *tmp_str;
	char *key;
	char *value;
	char *normal_str;
	int		sub_len;
	int		quote;


	quote = 0;
	new_str = NULL;
	sub_len = 0;
	i = 0;
	while (str[i])
	{
		// update_quote(str[i], &quote);
		// skip_single_quote_area(str, &i);
		while (str[i] && str[i] != '$')
		{
			update_quote(str[i], &quote);
			while (str[i] && quote == 1)
			{
				i++;
				sub_len++;
				update_quote(str[i], &quote);
			}
			i++;
			sub_len++;
		}
		// printf("sub_len: %d | i: %d\n", sub_len, i);
		if (sub_len != 0)
		{
			normal_str = ft_substr(str, i - sub_len, sub_len);
			// printf("normal_str: [%s]\n", normal_str);
			if (!normal_str)
				return (free(new_str), NULL);
			tmp_str = new_str;
			new_str = ft_strjoin(new_str, normal_str);
			if (!new_str)
				return (free(tmp_str), free(normal_str), NULL);
			free(normal_str);
			free(tmp_str);
			sub_len = 0;
		}
		if (quote != 1 && str[i] && str[i] == '$')
		{
			key = extract_target_key(str + i + 1);
			// printf("key: [%s]\n", key);
			value = env_get_value(env, key);
			// printf("value: %s\n", value);
			if (value)
			{
				tmp_str = new_str;
				new_str = ft_strjoin(new_str, value);
				if (!new_str)
					return (free(tmp_str), NULL);
				free(tmp_str);
			}
			i += ft_strlen(key) + 1;
		}
	}
	// printf("\033[32mfin: [%s]\033[0m\n", new_str);
	// free(str);
	return new_str;
}

char *join_arr(char **arr)
{
	int i;
	char *joined_arr = NULL;
	char *tmp;

	i = -1;
	while (arr[++i])
	{
		tmp = joined_arr;
		joined_arr = ft_strjoin(joined_arr, arr[i]);
		if (!joined_arr)
			return (free(tmp), NULL);
		free(tmp);

		tmp = joined_arr;
		joined_arr = ft_strjoin(joined_arr, " ");
		if (!joined_arr)
			return (free(tmp), NULL);
		// joined_arr = tmp;
		free(tmp);
	}
	return (joined_arr);
}


void free_2d_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}


char *trim_quotes(char *str)
{
		int i;
		char type = '\0';
		char *result = NULL;
		char *sub_str;
		char *tmp_result;
		int start;

		i = 0;
		while (str[i])
		{
			if (str[i] && (str[i] == '\'' || str[i] == '\"'))
			{
				type = str[i];
				i++;
				start = i;
				while (str[i] && str[i] != type)
					i++;
				sub_str = ft_substr(str, start, i - start);
				if (!sub_str)
					return (free(result), NULL);
				tmp_result = result;
				result = ft_strjoin(result, sub_str);
				(free(tmp_result), free(sub_str));
				if (!result)
					return (free(sub_str), NULL);
				i++;
			}
			else
			{
				type = '\0';
				start = i;
				while (str[i] && str[i] != '\'' && str[i] != '\"')
					i++;
				sub_str = ft_substr(str, start, i - start);
				if (!sub_str)
					return (free(result), NULL);
				tmp_result = result;
				result = ft_strjoin(result, sub_str);
				(free(tmp_result), free(sub_str));
				if (!result)
					return (free(tmp_result), NULL);
			}
		}
	return (result);
}
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
		// printf(">%s\n", commands_array[i]);
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

	char *joined_arr = join_arr(*arr);
	list = split(joined_arr);
	if (!list)
		return (free(joined_arr), 0);
	tmp = *arr;
	*arr = linked_list_to_double_array(list);	
	if (!*arr)
		return (0);
	return (1);
}

void expand_file_names(t_env *env, t_red *redirections)
{
	t_red *red;
	char *tmp;
	t_node *list = NULL;
	char **res;

	red = redirections;
	while (red)
	{
		if (red->type != HER_DOC)
		{
			tmp = do_expantion(env, red->data);
			tmp = trim_quotes(tmp);
			list = split(tmp);
			if (list->next != NULL)
				red->data = NULL;
			else
				red->data = list->data; 
		}
		red = red->next;
	}
}

int expand(t_env *env, t_tree *root)
{
	expand_commands(env, &root->data);
	expand_file_names(env, root->redirections);
	return 1;
}
