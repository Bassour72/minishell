/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:00:01 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 14:21:29 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

static int	count_double_dots(const char *path)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (path[i])
	{
		if (path[i] == '.' && path[i + 1] == '.')
			count++;
		i++;
	}
	return (count);
}

char	*build_logical_path(char *base, char *suffix)
{
	perror(ERR_GETCWD_FAIL);
	if (ft_strlen(suffix) > 2)
		return (ft_strjoin(base, suffix));
	return (ft_strjoin(base, "/.."));
}

int	free_all(char *a, char *b, char *c)
{
	if (a)
		free(a);
	if (b)
		free(b);
	if (c)
		free(c);
	return (0);
}

char	*get_env_path_parent(const char *path)
{
	char	*parent;
	int		len;
	int		i;

	len = ft_strlen(path);
	while (len > 1 && path[len - 1] == '/')
		len--;
	i = len - 1;
	while (i >= 0 && path[i] != '/')
		i--;
	if (i <= 0)
		return (ft_strdup("/"));
	parent = malloc(i + 1);
	if (!parent)
		return (NULL);
	ft_strlcpy(parent, path, i + 1);
	return (parent);
}

int	handle_cd_failure(t_env **env, char *arg, char *logical_pwd)
{
	char	*fallback;
	char	*next;
	int		steps;
	int		i;

	if (!logical_pwd)
		return (1);
	fallback = ft_strdup(logical_pwd);
	steps = count_double_dots(arg);
	i = 0;
	while (i++ < steps)
	{
		next = get_env_path_parent(fallback);
		if (!next)
			return (free(fallback), 1);
		free(fallback);
		fallback = next;
	}
	if (access(fallback, X_OK) == 0 && chdir(fallback) == 0)
	{
		set_env_var("OLDPWD", logical_pwd, env);
		set_env_var("PWD", fallback, env);
		return (free(fallback), 0);
	}
	return (free(fallback), 1);
}
