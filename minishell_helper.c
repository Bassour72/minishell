/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:10:36 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 16:04:46 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"

void	handle_sigint_prompt(int sig)
{
	(void)sig;
	if (waitpid(-1, NULL, WNOHANG) == 0)
		return ;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_signal_status = 1;
}

int	empty(char *str)
{
	char	*tmp;

	tmp = str;
	while (*str && ft_isspace(*str))
		str++;
	if (!*str)
	{
		free(tmp);
		return (1);
	}
	return (0);
}

void	handle_heredoc_on_fail(t_env **env_list, t_tree *tree)
{
	update_env_exit_status(env_list, 1);
	close_heredoc_fds(tree);
	free_tree(tree);
	g_signal_status = 0;
}

int	handle_heredoc(t_tree *root, t_env **env_list)
{
	int	state;

	enforce_heredoc_limit(root, env_list);
	state = prepare_heredocs(root, env_list);
	if (state != 0)
		handle_heredoc_on_fail(env_list, root);
	return (state);
}

void	is_input_null(char *input, t_env *env_list)
{
	int	status;

	if (input == NULL)
	{
		write(1, "exit\n", 5);
		status = ft_atol(get_env_value(EXIT_STATUS_KEY, env_list));
		check_non_interactive_exit(NULL, &env_list, status, true);
	}
}
