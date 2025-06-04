
#include "../../include/parsing.h"

t_expand_node *join_two_lists(t_expand_node *list_1, t_expand_node *list_2)
{
	t_expand_node *tmp;

	if (!list_1)
		return (list_2);
	tmp = list_1;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = list_2;
	return (list_1);
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

t_expand_node *split_tokens_into_nodes(t_expand_token *tokens)
{
	t_expand_node *main_list = NULL;
	t_expand_node *sub_list;

	// print_expand_tokens(tokens);
	int joinable = 0;
	while (tokens)
	{
		if (tokens->data && tokens->data[0] && tokens->split)
		{
			sub_list = expand_split(tokens->data, ' ', tokens->join);//todo protect
			if (!sub_list)
				return (free_expand_list_nodes(main_list), NULL);
			main_list = join_two_lists(main_list, sub_list);
		}
		else if (tokens->data)
		{
			main_list = append_str_to_list(main_list, tokens->data, tokens->join);
			if (!main_list)
				return (NULL);
		}
		tokens = tokens->next;
	}
	return (main_list);
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
	if (arr)
	{
		for(int i = 0; arr[i]; i++)
			free(arr[i]);
		free(arr);
	}
}



int expand(char ***new_args, char **old_args, t_env *env)
{
	t_expand_token *tokens;
	t_expand_node *nodes_list;
	char *line;

	nodes_list = NULL;
	tokens = NULL;
	// if (!old_args)
	// 	return (NULL);
	if (join_arr(old_args, &line) == R_FAIL)
		return (R_FAIL);
	// free(line);
	// line = ft_strdup("export $#  gg=$USER");
	
		if (tokenize(line, &tokens, env) == R_FAIL)
			return (R_FAIL);

	// free(line);
	// return NULL;
	// print_expand_tokens(tokens);
	nodes_list = split_tokens_into_nodes(tokens);
	// for(t_expand_node *tmp = nodes_list; tmp; tmp = tmp->next)
		// printf(">>{%s}\n", tmp->data); 
	free_expand_tokens_list(tokens);
	free(line);
	if (!nodes_list)
		return (R_FAIL);
	build_args_list_from_nodes_by_joining(nodes_list);
	if (expand_list_to_array(new_args, nodes_list) == R_FAIL)
		return (free_expand_list_nodes(nodes_list), R_FAIL);
	free_expand_list_nodes(nodes_list);
	
	return (R_SUCCESS);
}

