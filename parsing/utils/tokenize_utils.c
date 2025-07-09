/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:30:33 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 15:17:08 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

void	free_tokens_list(t_token *tokens)
{
	int		i;
	t_token	*list;

	if (!tokens)
		return ;
	list = tokens;
	i = -1;
	while ((list + ++i)->data)
	{
		free((list + i)->data);
	}
	free(tokens);
}

void	free_splitted_input(t_node *splitted_inpt)
{
	t_node	*tmp;

	while (splitted_inpt)
	{
		tmp = splitted_inpt;
		splitted_inpt = splitted_inpt->next;
		free(tmp->data);
		free(tmp);
	}
}

int	create_token(t_token *token, char *data, int type)
{
	token->data = ft_strdup(data);
	if (!token->data)
		return (perror("error"), R_FAIL);
	token->type = type;
	token->is_listed = 0;
	return (R_SUCCESS);
}

int	count_splitted_input_nodes(t_node *splitted_input)
{
	int	count;

	count = 0;
	while (splitted_input)
	{
		count++;
		splitted_input = splitted_input->next;
	}
	return (count);
}
