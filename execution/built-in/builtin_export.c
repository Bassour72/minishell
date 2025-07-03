#include "../../include/execution.h"

static  int ft_strcmp_v2(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static int	is_valid_identifier(const char *identifier, int *is_append_mode)
{
	int	i;
	int count_plus;
	if (!identifier)
		return (1);
	if (!ft_isalpha(identifier[0]) && identifier[0] != '_')
		return (1);
	i = 1;
	count_plus = 0;
	while (identifier[i] != '\0' && identifier[i] != '=')
	{
		if (identifier[i] == '+')
		{
			if (identifier[i + 1] != '\0' && identifier[i + 1] == '=')
				count_plus++;
			else
				return (1);
		}
		else if (!ft_isalnum(identifier[i]) && identifier[i] != '_') //todo
			return (1);
		if (count_plus > 1)
			return (1);
		i++;
	}
	*is_append_mode = count_plus;
	return (0);
}
static char *get_env_key(const char *identifier)
{
	char *new_key;
	int len = 0;
	int i;
	if (!identifier)
		return NULL;
	while (identifier[len] && identifier[len] != '=' && identifier[len] != '+')
		len++;

	new_key = malloc(len + 1);
	if (!new_key)
		return NULL;
	i = 0;
	while (i < len)
	{
		new_key[i] = identifier[i];
		i++;
	}
	new_key[i] = '\0';

	return new_key;
}

static char *get_env_value1(const char *identifier)
{
	int i = 0;

	if (!identifier)
		return (NULL);

	while (identifier[i] && identifier[i] != '=')
		i++;

	if (!identifier[i])
		return (NULL);
	return (ft_strtrim(identifier + i + 1, "\""));
}

static void swap_node(t_env *a, t_env *b)
{
	if (!a || !b)
		return;
	char *temp_key = a->key;
	char *temp_value = a->value;
	bool temp_exported = a->exported;
	a->key = b->key;
	a->value = b->value;
	a->exported = b->exported;
	b->key = temp_key;
	b->value = temp_value;
	b->exported = temp_exported;
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
			return (NULL);
		new_node->key = NULL;
		new_node->value = NULL;
		if (env_list->key)
			new_node->key = ft_strdup(env_list->key);
		if (env_list->value)
			new_node->value = ft_strdup(env_list->value);
		new_node->exported = env_list->exported;
		new_node->next = NULL;
		list_env_add_back(&copy, new_node);
		env_list = env_list->next;
	}
	return (copy);
}

static t_env *sort_env_list(t_env *env_list)
{
	bool swapped;
	t_env *ptr;

	if (!env_list)
		return (NULL);
	swapped = true;

	while (swapped)
	{
		swapped = false;
	
		ptr = env_list;
		while (ptr->next)
		{
			if (ft_strcmp_v2(ptr->key, ptr->next->key) > 0)
			{
				swap_node(ptr, ptr->next);
				swapped = true;
			}
			ptr = ptr->next;
		}
	}
	//printf("end.....................\n");
	return (env_list);
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
		if (tmp->value && tmp->is_remove != 0)
		{
			if (tmp->exported)
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		}
		else
		{
			if (tmp->exported)
				printf("declare -x %s\n", tmp->key);
		}
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

static void	update_existing_env(t_env *existing, char *new_value, int append_mode)
{
	char *temp_value;

	if (new_value)
	{
		if (append_mode)
		{
			temp_value = ft_strjoin(existing->value, new_value);
			free(existing->value);
			free(new_value);
			existing->value = temp_value;
			return ;
		}
		free(existing->value);
		existing->value = new_value;
	}
}

static void	add_new_env_node(t_env **env_list, char *new_key, char *new_value)
{
	t_env	*new_node;
	t_env	*ptr;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = new_key;
	new_node->value = new_value;
	new_node->exported = true;
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


int add_env_without_appned(t_env **env_list, char *new_key, char	*new_value)
{
	t_env	*existing;
	existing = is_exist_env(*env_list, new_key);
	if (existing)
	{
		update_existing_env(existing, new_value, 0);
		free(new_key);
		return (1);
	}
	add_new_env_node(env_list, new_key, new_value);
	return (1);
}



int append_env_node(t_env **env_list, char *new_key, char	*new_value)
{
	t_env	*existing;
	
	existing = is_exist_env(*env_list, new_key);
	if (existing)
	{
		update_existing_env(existing,new_value, 1);
		free(new_key);
		return (1);
	}
	add_new_env_node(env_list, new_key, new_value);
	return (1);
}

static int	add_env(char *arg, t_env **env_list)
{
	t_env	*existing;
	char	*new_key;
	char	*new_value;
	int is_append_mode;

	if (!arg)
		return (1);
	is_append_mode = 0;
	if (is_valid_identifier(arg, &is_append_mode) == 1)
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	new_key = get_env_key(arg);
	if (!new_key)
		return (1);
	new_value = get_env_value1(arg);
	if (is_append_mode != 1)
	{
		add_env_without_appned(env_list, new_key,new_value);
		return (1);
	}
	return (append_env_node(env_list, new_key, new_value));
}

int export_command_builtin(t_tree *root, t_env **env_list)
{
	int i;
	if (env_list == NULL)
		return ((0));
	else if (root->data[1] != NULL)
	{
		i =  1;
		while (root->data[i] != NULL)
		{
			add_env(root->data[i], env_list);
			i++;
		}
		return (0);
	}
	else 
		print_env_export_sort(*env_list);
	return (0);
}

