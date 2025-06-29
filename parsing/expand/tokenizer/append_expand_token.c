#include "../../../include/parsing.h"

int append_expand_token(t_expand_token **tokens, char *data, t_expand_type type, int split, int join)
{
	t_expand_token *new_token;
	t_expand_token *tmp;

	if (!data)
		return (R_SUCCESS);
	// printf("\033[32mnew token data: [%s]| size:[%d]\033[0m\n", data, ft_strlen(data));
	new_token = malloc(sizeof(t_expand_token));
	if (!new_token)
		return (perror("error: "), free(data), R_FAIL);
	new_token->data = data;
	// printf(">>>{%s}\n",data);
	new_token->join = join;
	new_token->split = split;
	// if (split == 1 || split == 2 )
	// 	new_token->split = 0;
	new_token->next = NULL;
	new_token->prev = NULL;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
		new_token->prev = tmp;
	}
	return (R_SUCCESS);
}
