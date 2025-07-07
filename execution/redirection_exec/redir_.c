/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 21:46:05 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 22:07:37 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

int	apply_redirections(t_red *redir, t_env **env_list)
{
	while (redir)
	{
		if (redir->is_ambiguous == 1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(redir->data, STDERR_FILENO);
			ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
			return (1);
		}
		if (apply_single_redirection(redir, env_list) == 1)
			return (1);
		redir = redir->next;
	}
	return (0);
}
