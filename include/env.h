/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:44:39 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 17:46:09 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "../_libft/libft.h"
# include <stdio.h>

# define EXIT_STATUS_KEY "exit_status@gmail.com"
# define R_FAIL 0
# define R_SUCCESS 1

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	int				is_remove;
	char			*temp_key;
	struct s_env	*next;
}				t_env;

void	free_env_list(t_env *list);
int		env_generate(t_env **env_list, char **env);
char	*env_get_value(t_env *list, char *key);
int		create_env_node(t_env **list, char *key, char *value);
int		set_pwd_and_physical_pwd_(t_env **env_list);
int		update_env_exit_status(t_env **env_list, int status);
int		init_env(t_env **env_list);

#endif