#ifndef EXECUTION_H
# define EXECUTION_H
#include "parsing.h"
#include <sys/wait.h>

int		execution(t_tree *root, char **env);
int		builtin_echo(t_tree *root);
char    *get_binary_file_path(t_tree *root, char **env);
#endif