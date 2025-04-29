/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:57:18 by massrayb          #+#    #+#             */
/*   Updated: 2025/04/29 15:24:45 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parsing.h"

int	empty(char *str)
{
	if (!str[0])
		return 1;
	while (*str && *str == ' ')
	{
		str++;
	}
	return (!*str);
}

int main(void)
{
	t_tree *tree = NULL;
	char *input;
	input = NULL;
	while (1)
	{
		//get the input from the terminal
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (empty(input))
		{
			free(input);
			continue ;
		}

		add_history(input);

		//parsing the input and generate the tree
		tree = parser(tree, input);
		//take the tree and execute here
		
		


		
		//-----------------------------
	}

	return (0);
}