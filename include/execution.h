#ifndef EXECUTION_H
# define EXECUTION_H
#include "parsing.h"
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


typedef struct s_pipe
{
    int fds[2];         // fds[0] for reading, fds[1] for writing
    struct s_pipe *next;
    struct s_pipe *prev;
} t_pipe;
// int exec_pipe(t_tree *root, char **env, int input_fd);
int		execution(t_tree *root, char **env, t_env **env_list);
void    execute_command(t_tree *root, char **env, t_env **env_list);
char    *get_binary_file_path(t_tree *root, char **env);
int		builtin_echo(t_tree *root);
int	cd_change_working_directory(t_tree *root);
int handle_shlvl(char *argv0, t_env **env_list);
int pwd_print_working_directory(t_tree *root);
int is_builtin(char *command);
int execute_builtin(t_tree *root, char **env, t_env **env_list);
int	env_environment(t_tree *root, char **env);
char *get_binary_file_path(t_tree *root, char **env);
int	exit_exe(t_tree *root);
int	export_command_builtin(t_tree *root, t_env **env_list);
int	builtin_unset_environment(t_tree *root, t_env **env_list, char **env);
void execute_command(t_tree *root, char **env, t_env **env_list);
void free_tree_exe(t_tree *root);
int exec_pipe(t_tree *root, char **env, int input_fd, t_env **env_list);
void write_here_doc(int fd, char *limiter_nl);
int execution(t_tree *root, char **env, t_env **env_list);
int setup_redirections(t_tree *root);
void free_tree_exe(t_tree *root);
void write_heredoc(int fd, const char *limiter);
void create_heredoc(t_red *redir);
void prepare_heredocs(t_tree *root);
void apply_redirections(t_red *redir);
#endif