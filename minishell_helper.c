/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:10:36 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 21:23:24 by ybassour         ###   ########.fr       */
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
	g_exit_status = 1;
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
	g_exit_status = 0;
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
		close(2);
		close(0);
		status = ft_atoi(get_env_value(EXIT_STATUS_KEY, env_list));
		free_env_list(env_list);
		exit(status);
	}
}
