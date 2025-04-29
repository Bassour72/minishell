/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:48:41 by massrayb          #+#    #+#             */
/*   Updated: 2025/04/23 09:37:11 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"
static t_red *new_redirection(char *data, t_type type)
{
	t_red *new_red;

	if (!data)
		return (NULL);
	new_red = malloc(sizeof(t_red));
	if (!new_red)
		return (NULL);
	new_red->data = data;
	new_red->type = type;
	return (new_red);
}

void append_redirection(t_red *redirections_list, char *data, t_type type)
{
	if (!redirections_list)
		return (NULL);
	while (redirections_list->next)
	{
		redirections_list = new_redirection(data, type);
		redirections_list = redirections_list->next;
	}
}