/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 00:09:14 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 18:44:40 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

void	free_expand_list_nodes(t_expand_node *list)
{
	t_expand_node	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}

int	check_empty(t_expand_token *tokens)
{
	while (tokens)
	{
		if (tokens->data && tokens->data[0])
			return (0);
		tokens = tokens->next;
	}
	return (1);
}

int	recover_quotes(char ***new_args)
{
	int	i;
	int	j;

	i = -1;
	while ((*new_args)[++i])
	{
		j = -1;
		while ((*new_args)[i][++j])
		{
			if ((*new_args)[i][j] == DOUBLE_QUOTE)
				(*new_args)[i][j] = '\"';
			else if ((*new_args)[i][j] == SINGLE_QUOTE)
				(*new_args)[i][j] = '\'';
		}
	}
	if (!(*new_args)[0])
	{
		1 && (free(*new_args), *new_args = ft_calloc(2, sizeof(char *)));
		if (!*new_args)
			return (R_FAIL);
		(*new_args)[0] = ft_strdup("");
		if (!(*new_args)[0])
			return (perror("error: "), free(*new_args), R_FAIL);
	}
	return (R_SUCCESS);
}

static int	generate_tokens(char ***new_args, \
t_expand_token **tokens, t_env *env)
{
	char	*line;

	*tokens = NULL;
	if (join_arr(*new_args, &line) == R_FAIL)
		return (R_FAIL);
	free_2d_arr(*new_args);
	*new_args = NULL;
	if (tokenize(line, tokens, env) == R_FAIL)
		return (free(line), R_FAIL);
	free(line);
	return (R_SUCCESS);
}

int	expand(char ***new_args, t_env *env)
{
	t_expand_token	*tokens;
	t_node			*splited_line;
	char			*new_line;

	splited_line = NULL;
	if (!*new_args)
		return (R_SUCCESS);
	if (generate_tokens(new_args, &tokens, env) == R_FAIL)
		return (R_FAIL);
	if (check_empty(tokens))
		return (R_CONTINUE);
	if (expand_tokens_to_line(&new_line, tokens) == R_FAIL)
		return (free_expand_tokens_list(tokens), R_FAIL);
	free_expand_tokens_list(tokens);
	if (expand_split2(&splited_line, new_line) == R_FAIL)
		return (free(new_line), R_FAIL);
	free(new_line);
	if (expand_list_to_array(new_args, splited_line) == R_FAIL)
		return (R_FAIL);
	if (wildcard(new_args) == R_FAIL)
		return (R_FAIL);
	if (remove_non_printable_characters(new_args) == R_FAIL)
		return (R_FAIL);
	return (recover_quotes(new_args));
}
