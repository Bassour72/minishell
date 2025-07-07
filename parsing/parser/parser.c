/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:11:46 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 17:14:21 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

void	clean_the_flat_tree(t_flat_tree *flat_tree)
{
	t_flat_tree	*tmp;

	while (flat_tree)
	{
		tmp = flat_tree;
		flat_tree = flat_tree->backup_next;
		if (tmp->tree_node->empty == -1)
			free(tmp->tree_node);
		free(tmp);
	}
}

int	parser(t_tree **tree, char *input, t_env **env)
{
	t_token		*tokenized_input;
	t_flat_tree	*flat_tree;

	tokenized_input = NULL;
	if (!validate_quotes(input))
		return (free(input), *tree = NULL, R_SUCCESS);
	if (tokenizer(&tokenized_input, input) == R_FAIL)
		return (free_tokens_list(tokenized_input), R_FAIL);
	if (validate_sytax(tokenized_input) == R_FAIL)
	{
		if (update_env_exit_status(env, 258) == R_FAIL)
			return (free_tokens_list(tokenized_input), R_FAIL);
		return (*tree = NULL, free_tokens_list(tokenized_input), R_SUCCESS);
	}
	flat_tree = create_flat_tree(tokenized_input);
	free_tokens_list(tokenized_input);
	if (!flat_tree)
		return (R_FAIL);
	*tree = init_tree(flat_tree);
	clean_the_flat_tree(flat_tree);
	return (R_SUCCESS);
}
