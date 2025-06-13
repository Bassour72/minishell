
#include "../../include/parsing.h"

void join_two_lists(t_expand_node **list_1, t_expand_node *list_2)
{
	t_expand_node *tmp;

	if (!*list_1)
		*list_1 = list_2;
	else
	{
		tmp = *list_1;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = list_2;
	}
}

t_expand_node *append_str_to_list(t_expand_node *list_1, char *str, int join)
{
	t_expand_node *tmp;
	t_expand_node	*new_node;

	new_node = malloc(sizeof(t_expand_node));
	if (!new_node)
		return (perror("error: "),free_expand_list_nodes(list_1) , NULL);
	new_node->joinable = join;
	new_node->next = NULL;
	new_node->data = ft_strdup(str);
	if (!new_node->data)
		return (perror("error: "), free_expand_list_nodes(list_1), free(new_node), NULL);
	if (!list_1)
		return (new_node);
	tmp = list_1;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (list_1);
}

int split_tokens_into_nodes(t_expand_node **expanded_list,  t_expand_token *tokens)
{
	t_expand_node *main_list = NULL;
	t_expand_node *sub_list;

	while (tokens)
	{
		sub_list = NULL;
		if (tokens->data && tokens->data[0] && tokens->split)
		{
			// printf("NEED to split [%s]\n", tokens->data);
			if (expand_split(&sub_list, tokens->data, ' ', tokens->join) == R_FAIL)//todo protect
				return (free_expand_list_nodes(*expanded_list), R_FAIL);
			join_two_lists(expanded_list, sub_list);
		}
		else if (tokens->data && tokens->data[0])
		{
			// printf("NO need to split [%s]\n", tokens->data);
			*expanded_list = append_str_to_list(*expanded_list, tokens->data, tokens->join);
			if (!*expanded_list)
				return (R_FAIL);
		}
		tokens = tokens->next;
	}
	return (R_SUCCESS);
}

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

static void free_double_array(char **arr) //add to utils
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}



int expand(char ***new_args, t_env *env)
{
	t_expand_token *tokens;
	t_expand_node *nodes_list;
	char *line;

	nodes_list = NULL;
	tokens = NULL;
	// if (!old_args)
	// 	return (NULL);
	if (join_arr(*new_args, &line) == R_FAIL)
		return (R_FAIL);

	char **tmp = *new_args;
	*new_args = NULL;
	free_double_array(tmp);
	// free(line);
	// line = ft_strdup("export $#  gg=$USER");
	
	if (tokenize(line, &tokens, env) == R_FAIL)
			return (free(line), R_FAIL);

	// free(line);
	// return NULL;
	// print_expand_tokens(tokens);
	if (split_tokens_into_nodes(&nodes_list,  tokens) == R_FAIL)
		return (free(line), free_expand_tokens_list(tokens), R_FAIL);
	// for(t_expand_node *tmp = nodes_list; tmp; tmp = tmp->next)
	free_expand_tokens_list(tokens);
	free(line);

	if (build_args_list_from_nodes_by_joining(nodes_list) == R_FAIL)
		return (free_expand_list_nodes(nodes_list), R_FAIL);
	// for(t_expand_node *tmp = nodes_list; tmp; tmp = tmp->next)
	// {
	// 	printf("[%s]\n", tmp->data);
	// }
	if (expand_list_to_array(new_args, nodes_list) == R_FAIL)
		return (free_expand_list_nodes(nodes_list), R_FAIL);
	free_expand_list_nodes(nodes_list);
	// for(int i = 0; (*new_args)[i]; i++)
	// 	printf("(%s)\n", (*new_args)[i]);
	return (R_SUCCESS);
}

