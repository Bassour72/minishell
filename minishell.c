/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:57:18 by massrayb          #+#    #+#             */
/*   Updated: 2025/05/02 19:44:30 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"

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

void f()
{
	system("leaks -q minishell");
}

int main(void)
{
	atexit(f);
	t_tree	*tree = NULL;
	char	*input;
	input = NULL;
	while (1)
	{
		//get the input from the terminal
		input = readline("minishell$ ");
		// printf("address : %p\n", input);
		if (!input)
			break ;
		if (empty(input))
		{
			free(input);
			continue ;
		}
	
		add_history(input);

		tree = parser(tree, input);
		execution(tree);
		free_tree(tree); //note this is for freeing the tree
		//take the tree and execute here
		
		


		
		//-----------------------------
	}

	return (0);
}