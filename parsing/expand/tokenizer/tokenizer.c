/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 23:23:41 by massrayb          #+#    #+#             */
/*   Updated: 2025/06/29 23:35:30 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int	is_variable(char *str, int i)
{
	return (str[i] == '$' && str[i + 1] && str[i + 1] != '\"' && \
			str[i + 1] != '\'' && str[i + 1] != ' ');
}

static int	handle_quotes(char *str, int *i, t_expand_token **tokens, \
	t_env *env)
{
	if (str[*i] == '\'')
	{
		if (single_quote_expander(str + *i, i, tokens) == R_FAIL)
			return (R_FAIL);
	}
	else if (str[*i] == '\"')
	{
		if (double_quote_expander(str + *i, i, tokens, env) == R_FAIL)
			return (R_FAIL);
	}
	return (R_SUCCESS);
}

int	tokenize(char *str, t_expand_token **tokens, t_env *env)
{
	int	i;
	int	state;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (handle_quotes(str, &i, tokens, env) == R_FAIL)
				return (R_FAIL);
		}
		else if (is_variable(str, i))
		{
			state = normal_variable_expander(str, &i, tokens, env);
			if (state == R_CONTINUE)
				continue ;
			else if (state == R_FAIL)
				return (R_FAIL);
		}
		else if (str[i] == ' ')
			i++;
		else if (normal_expander(str + i, &i, tokens) == R_FAIL)
			return (R_FAIL);
	}
	return (R_SUCCESS);
}
