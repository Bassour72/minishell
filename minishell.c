/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:57:02 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 20:44:56 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"

int	g_exit_status = 0;

static int	shell_loop(t_env **env_list, t_tree **tree, \
char **input, int *status)
{
	while (1)
	{
		(signal(SIGINT, handle_sigint_prompt), disable_echoctl());
		if (dup2(2, 0) == -1)
			return (perror("error: "), R_FAIL);
		*input = readline("minishell$ ");
		enable_echoctl();
		if (g_exit_status == 1)
			(update_env_exit_status(env_list, 1), g_exit_status = 0);
		is_input_null(*input, *env_list);
		if (empty(*input))
			continue ;
		add_history(*input);
		if (parser(tree, *input, env_list) == R_FAIL)
			return (1);
		if (!*tree)
			continue ;
		if (handle_heredoc(*tree, env_list) != 0)
			continue ;
		*status = execution(*tree, env_list);
		update_env_exit_status(env_list, *status);
		free_tree(*tree);
		g_exit_status = 0;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env_list;
	int		status;
	t_tree	*tree;
	char	*input;
	int		loop_status;

	(void)ac;
	env_list = NULL;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		exit (1);
	if (env_generate(&env_list, envp) == R_FAIL)
		return (1);
	if (handle_shlvl(av[0], &env_list) == R_FAIL)
		return (1);
	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
	status = shell_loop(&env_list, &tree, &input, &loop_status);
	free_env_list(env_list);
	return (status);
}
