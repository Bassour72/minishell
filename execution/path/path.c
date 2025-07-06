#include "../../include/execution.h"

static void	free_split_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static char	*search_command_in_path(char *command, char **binaries_path)
{
	int		i;
	char	*full_path;
	char	*temp;

	i = 0;
	while (binaries_path[i])
	{
		full_path = ft_strjoin(binaries_path[i], "/");
		temp = ft_strjoin(full_path, command);
		free(full_path);
		if (access(temp, X_OK) == 0)
		{
			free_split_array(binaries_path);
			return (temp);
		}
		free(temp);
		i++;
	}
	free_split_array(binaries_path);
	return (NULL);
}

static char	*join_binary_path_with_command(char *command, char *binary_path)
{
	char	**binaries_path;

	binaries_path = ft_split(binary_path, ':');
	if (!binaries_path)
		return (NULL);
	return (search_command_in_path(command, binaries_path));
}

char	*get_binary_file_path(t_tree *root, t_env **env_list)
{
	char	*default_path;
	char	*binary_path;

	if (!root || !root->data || !root->data[0])
	{
		perror("Error: Empty command node");
		return (NULL);
	}
    if (root->data[0][0] == '\0')
		return (NULL);
	if (ft_strchr(root->data[0], '.') || ft_strchr(root->data[0], '/'))
	{
		 if (should_display_error(root->data[0], env_list, false) != 0)
		 	return (NULL);
		return (check_valid_command_path(root->data[0]));
	}
	default_path = extract_path_variable(env_list);
	binary_path = join_binary_path_with_command(root->data[0], default_path);
	free(default_path);
	return (binary_path);
}
