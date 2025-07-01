
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
	free(line);
	if (expand_tokens_to_line(&new_line, tokens) == R_FAIL)
		return (free_expand_tokens_list(tokens), R_FAIL);
	free_expand_tokens_list(tokens);
	if (expand_split2(&splited_line, new_line) == R_FAIL)
		return (free(new_line), R_FAIL);
	free(new_line);
	if (expand_list_to_array(new_args, splited_line) == R_FAIL)
		return (R_FAIL);
	if(wildcard(new_args) == R_FAIL)
		return (R_FAIL);
	if(remove_non_printable_characters(new_args) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}

