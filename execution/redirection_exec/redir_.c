/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 21:46:05 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 18:02:41 by ybassour         ###   ########.fr       */
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
			return (STATUS_ERROR);
		}
		if (apply_single_redirection(redir, env_list) == 1)
			return (STATUS_ERROR);
		redir = redir->next;
	}
	return (STATUS_OK);
}
