#include "../../../include/parsing.h"

static int is_valid_key(char *key)
{
	int	i;

	if (!key[0])
		return (0);
	i = -1;
	while (key[++i] && key[i] != '=')
	{
		if (is_valid_key_char(key[i], i) == 0)
		{
			// printf("not valid\n");
			return (0);
		}
	}
	return (1);
}

static int is_befor_var_valid(t_expand_token *tokens)
{
	t_expand_token *target; //must be null
	t_expand_token *equal; //must be =
	t_expand_token *key; 

	while (tokens->next)
		tokens = tokens->next;

	if (ft_strcmp(tokens->data, "=") == 0)
		tokens = tokens->prev;

	if (tokens->join == 1 && tokens->split == 1 && is_valid_key(tokens->data))
			return (1);

	// printf("not valid\n");	
	return (0);
}

static int is_export(char *line)
{
	if (ft_memcmp("export ", line, 7) == 0)
		return (1);
	return (0);
}

int expand_variable(t_expand_funcs_data e)
{
	int split;
	int join;

	join = 0;


	// if (!is_valid_key_char(e.str[*e.i + 1], 0) && e.str[*e.i + 1] != '?')
	// {
	// 	printf("fuck this shitlk jfbSKDH FNLVKJDSBV; KSBlkBSFLKSDbflkSBFK\n");
	// 	if (*e.quote == 2)
	// 	{
	// 		if (extract_word_of_double_quote(e.str + *e.i, e.i, e.data) == R_FAIL)
	// 			return (free_expand_tokens_list(*e.tokens), R_FAIL);
	// 		if (e.str[*e.i + 1] != ' ')
	// 			join = 1;
	// 		if (append_expand_token(e.tokens, e.data, e_WORD, *e.quote, join) == R_FAIL)
	// 			return (free_expand_tokens_list(*e.tokens), R_FAIL);
	// 	}
	// 	if (e.str[*e.i + 1] == '\'' || e.str[*e.i + 1] =='\"')
	// 		*e.i++;
	// 	else
	// 		*e.i += 2;
	// 	// continue; //make a return value just for this condition
	// 	return (CONTINUE);
	// }

	split = *e.quote;
	printf("[%s]<<<<<<<<\n", e.str);
	if (is_export(e.str))
	{
		if (extract_var_value(e.str + *e.i + 1, e.i, e.data, e.env) == R_FAIL)
			return (free_expand_tokens_list(*e.tokens), R_FAIL);
		if (e.str[*e.i] != ' ' && e.str[*e.i] != '\"')
			join = 1;
		if (is_befor_var_valid(*e.tokens))
			split = 2;
			// printf("split: %d| {%s}\n", split , data);
		//this maybe is incomplit !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	else
	{
		
		if (extract_var_value(e.str + *e.i + 1, e.i, e.data, e.env) == R_FAIL)
			return (free_expand_tokens_list(*e.tokens), R_FAIL);
		if (e.str[*e.i] != ' ' && e.str[*e.i])
		{
			printf("{%d}[i %c] [i + 1 %c]\n", *e.i, e.str[*e.i], e.str[*e.i + 1]);
			join = 1;
		}
	}
	if (append_expand_token(e.tokens, *e.data, VAR, split, join) == R_FAIL)
		return (free_expand_tokens_list(*e.tokens), R_FAIL);
	return (R_SUCCESS);
}