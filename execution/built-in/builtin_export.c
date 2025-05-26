#include "../../include/execution.h"
#include <stdbool.h>

static int	is_valid_identifier(const char *identifier)
{
	int	i;

	if (!identifier)
		return (1);
	if (!ft_isalpha(identifier[0]) && identifier[0] != '_')
		return (1);
	i = 1;
	while (identifier[i] != '\0' && identifier[i] != '=')
	{
		if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (1);
		i++;
	}
	return (0);
}
static char *get_env_key(const char *identifier)
{
	char *new_key;
	int len = 0;
	int i = 0;

	if (!identifier)
		return NULL;

	while (identifier[len] && identifier[len] != '=')
		len++;

	new_key = malloc(len + 1);
	if (!new_key)
		return NULL;

	while (i < len)
	{
		new_key[i] = identifier[i];
		i++;
	}
	new_key[i] = '\0';

	return new_key;
}

static char *get_env_value(const char *identifier)
{
	int i = 0;

	if (!identifier)
		return (NULL);

	while (identifier[i] && identifier[i] != '=')
		i++;

	if (!identifier[i])
		return (NULL);
	return ft_strdup(ft_strtrim(identifier + i + 1, "\""));
}

static void swap_node(t_env *a, t_env *b)
{
	if (!a || !b)
		return;
	char *temp_key = a->key;
	char *temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

static void list_env_add_back(t_env **env_list, t_env *new_node_env)
{
	t_env *copy;

	if (!new_node_env)
		return;
	if (!(*env_list))
		*env_list = new_node_env;
	else
	{
		copy = *env_list;
		while (copy->next != NULL)
			copy = copy->next;
		copy->next = new_node_env;
	}
}

static t_env *copy_env_list(t_env *env_list)
{
	t_env *copy = NULL;
	t_env *new_node;

	while (env_list != NULL)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return NULL;
		new_node->key = env_list->key ? strdup(env_list->key) : NULL;
		new_node->value = env_list->value ? strdup(env_list->value) : NULL;
		new_node->next = NULL;
		list_env_add_back(&copy, new_node);
		env_list = env_list->next;
	}
	return copy;
}

static t_env *sort_env_list(t_env *env_list)
{
	bool swapped;
	t_env *ptr;

	if (!env_list)
		return NULL;
	do 
	{
		swapped = false;
		ptr = env_list;
		while (ptr->next)
		{
			if (strcmp(ptr->key, ptr->next->key) > 0)
			{
				swap_node(ptr, ptr->next);
				swapped = true;
			}
			ptr = ptr->next;
		}
	} while (swapped);
	return env_list;
}

static void print_env_export_sort(t_env *env_list)
{
	t_env *sorted_list;
	t_env *tmp;

	sorted_list = copy_env_list(env_list);
	sorted_list = sort_env_list(sorted_list);
	tmp = sorted_list;
	while (tmp)
	{
		if (tmp->value)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("declare -x %s\n", tmp->key);
		tmp = tmp->next;
	}
	free_env_list(sorted_list);
}

static t_env *is_exist_env(t_env *env_list, const char *new_key)
{
	t_env *tmp = env_list;

	while (tmp)
	{
		if (ft_strcmp(tmp->key, new_key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}


static void	add_env(t_tree *root, t_env **env_list)
{
	t_env *existing;
	t_env *new_node;
	t_env *ptr;
	char *new_key;
	char *new_value;

	if (!root || !root->data[1])
		return ;
	if (is_valid_identifier(root->data[1]) == 1)
	{
		printf("export: `%s': not a valid identifier\n", root->data[1]);
		return ;
	}
	new_key = get_env_key(root->data[1]);
	new_value = get_env_value(root->data[1]);
	existing = is_exist_env(*env_list, new_key);
	if (existing)
	{
		if (new_value)
		{
			free(existing->value);
			existing->value = new_value;
		}
		free(new_key);
		return ;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = new_key;
	new_node->value = new_value;
	new_node->next = NULL;
	if (!*env_list)
		*env_list = new_node;
	else
	{
		ptr = *env_list;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new_node;
	}
}

int export_command_builtin(t_tree *root, t_env **env_list)
{
	if (env_list == NULL)
	{
		
		printf("\n\n\n [%s]\n\n\n", "root->data[1]");
		sleep(1);
		return 0;
	}
	else if (root->data[1] != NULL)
	{

		printf("\n\n\n [%s]\n\n\n", "for add noe env");
		sleep(1);
		add_env(root, env_list);
		return 0;
	}
	else 
		print_env_export_sort(*env_list);
	return 0;
}

