#include "../../include/parsing.h"

// char *do_expantion(t_env *env, char *str)
// {
// 	int i;
// 	char *new_str;
// 	char *tmp_str;
// 	char *key;
// 	char *value;
// 	char *normal_str;
// 	int		sub_len;
// 	int		quote;


// 	quote = 0;
// 	new_str = NULL;
// 	sub_len = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		// update_quote(str[i], &quote);
// 		// skip_single_quote_area(str, &i);
// 		while (str[i] && str[i] != '$')
// 		{
// 			update_quote(str[i], &quote);
// 			while (str[i] && quote == 1)
// 			{
// 				i++;
// 				sub_len++;
// 				update_quote(str[i], &quote);
// 			}
// 			i++;
// 			sub_len++;
// 		}
// 		// printf("sub_len: %d | i: %d\n", sub_len, i);
// 		if (sub_len != 0)
// 		{
// 			normal_str = ft_substr(str, i - sub_len, sub_len);
// 			// printf("normal_str: [%s]\n", normal_str);
// 			if (!normal_str)
// 				return (free(new_str), NULL);
// 			tmp_str = new_str;
// 			new_str = ft_strjoin(new_str, normal_str);
// 			if (!new_str)
// 				return (free(tmp_str), free(normal_str), NULL);
// 			free(normal_str);
// 			free(tmp_str);
// 			sub_len = 0;
// 		}
// 		if (quote != 1 && str[i] && str[i] == '$')
// 		{
// 			key = extract_target_key(str + i + 1);
// 			// printf("key: [%s]\n", key);
// 			value = env_get_value(env, key);
// 			// printf("value: %s\n", value);
// 			if (value)
// 			{
// 				tmp_str = new_str;
// 				new_str = ft_strjoin(new_str, value);
// 				if (!new_str)
// 					return (free(tmp_str), NULL);
// 				free(tmp_str);
// 			}
// 			i += ft_strlen(key) + 1;
// 		}
// 	}
// 	// printf("\033[32mfin: [%s]\033[0m\n", new_str);
// 	// free(str);
// 	return new_str;
// }



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
