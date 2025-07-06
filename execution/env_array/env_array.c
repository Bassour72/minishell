#include "../../include/execution.h"

int env_list_len(t_env *env_list)
{
    int len;
	len = 0;
    while (env_list)
    {
        len++;
        env_list = env_list->next;
    }
    return (len);
}

char *create_env_string(t_env *node)
{
	size_t key_len;
    size_t value_len;
    size_t total_len;
	char *env_string;

    key_len =  0;
	if (node->key)
		key_len =  ft_strlen(node->key);

	value_len = 0;
	if (node->value)
    	value_len = ft_strlen(node->value);

    total_len = key_len + value_len + 2;

    env_string = malloc(total_len);
    if (!env_string)
        return (NULL);

    if (node->key)
        strcpy(env_string, node->key);//TODO FT_STRCPY
    else
        env_string[0] = '\0';
    strcat(env_string, "=");//TODO FT_STRCAT
    if (node->value)
        strcat(env_string, node->value);//TODO FT_STRCAT

    return (env_string);
}

char **gen_new_env(t_env *env_list)
{
    int len;
	int i;
	char **new_env;
    t_env *tmp;
	if (!env_list)
		return (NULL);
	len = env_list_len(env_list);
	tmp = env_list;
    new_env = malloc(sizeof(char *) * (len + 1));
    if (!new_env)
        return (NULL);
    tmp = env_list;
    i = 0;
    while (tmp)
    {
        new_env[i] = create_env_string(tmp);
        if (!new_env[i])
        {
            while (i > 0)
                free(new_env[--i]);
            free(new_env);
            return (NULL);
        }
        i++;
        tmp = tmp->next;
    }
    new_env[i] = NULL;
    return (new_env);
}
