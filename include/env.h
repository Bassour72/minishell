#ifndef ENV_H
#define ENV_H

#include "../_libft/libft.h"
#include <stdio.h>


typedef struct s_env
{
	char			*key;
	char			*value;
	int exported;
	struct s_env	*next;
}			t_env;

void	free_env_list(t_env *list);
int		env_generate(t_env **env_list, char **env);
char	*env_get_value(t_env *list, char *key);
void	print_env(t_env *env);

#endif