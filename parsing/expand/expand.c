
#include "../../include/parsing.h"


void free_expand_list_nodes(t_expand_node *list)
{
	t_expand_node *tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}
int clean_slitted_line(char *line)
{
	int	is_empty_var;
	int	i;

	is_empty_var = 1;
	i = 0;
	while(line[i])
	{
		if((unsigned char)line[i] != 255)
			is_empty_var = 0;
		i++;
	}
	if (is_empty_var)
		return (1);
	while (*line)
	{
		if((unsigned char)*line == 255)
			*line = ' ';
		line++;
	}
	return (0);
}

int	check_empty(t_expand_token *tokens)
{
	while (tokens)
	{
		if (tokens->data && tokens->data[0])
			return(0);
		tokens = tokens->next;
	}
	return (1);
}

static void recover_quotes(char **new_args)
{
	int	i;
	int	j;

	i = 0;
	while (new_args[i])
	{
		j = 0;
		while (new_args[i][j])
		{
			if (new_args[i][j] == DOUBLE_QUOTE)
				new_args[i][j] = '\"';
			else if (new_args[i][j] == SINGLE_QUOTE)
				new_args[i][j] = '\'';
			j++;
		}
		i++;
	}
}

int expand(char ***new_args, t_env *env)
{
	t_expand_token	*tokens;
	t_node			*splited_line;
	char			*line;
	char			*new_line;

	tokens = NULL;
	splited_line = NULL;
	if (join_arr(*new_args, &line) == R_FAIL)
		return (R_FAIL);
	free_2d_arr(*new_args);
	*new_args = NULL;
	if (tokenize(line, &tokens, env) == R_FAIL)
		return (free(line), R_FAIL);

	if (check_empty(tokens))
		return (R_CONTINUE);
	print_expand_tokens(tokens);
	free(line);
	if (expand_tokens_to_line(&new_line, tokens) == R_FAIL)
		return (free_expand_tokens_list(tokens), R_FAIL);
		// print(new_line);
	free_expand_tokens_list(tokens);
		
	// if (clean_slitted_line(new_line))
	// 	return (free(new_line), R_CONTINUE);
	if (expand_split2(&splited_line, new_line) == R_FAIL)
		return (free(new_line), R_FAIL);
		print("##################");
	free(new_line);
	if (expand_list_to_array(new_args, splited_line) == R_FAIL)
		return (R_FAIL);
	for(int i = 0; (*new_args)[i]; i++)
	{
		printf("<%s>[%d]\n", (*new_args)[i], ft_strlen((*new_args)[i]));
	}
	if(wildcard(new_args) == R_FAIL)
		return (R_FAIL);
	if(remove_non_printable_characters(new_args) == R_FAIL)
		return (R_FAIL);
	recover_quotes(*new_args);
	for(int i = 0; (*new_args)[i]; i++)
	{
		printf("(%s)[%d]\n", (*new_args)[i], ft_strlen((*new_args)[i]));
	}
	print("------------------------------------------------------------------------------------");
	return (R_SUCCESS);
}

