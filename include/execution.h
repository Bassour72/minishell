#ifndef EXECUTION_H
# define EXECUTION_H
#include "parsing.h"
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>


extern int g_exit_status;
typedef struct s_env_var 
{
    char *key;             // e.g., "PATH"
    char *value;           // e.g., "/usr/bin"
    int exported;         // true if visible to execve/env
    struct s_env_var *next;
} t_env_var;
typedef struct s_pipe
{
    int fds[2];         // fds[0] for reading, fds[1] for writing
    struct s_pipe *next;
    struct s_pipe *prev;
} t_pipe;
// int exec_pipe(t_tree *root, char **env, int input_fd);
int		execution(t_tree *root, char **env, t_env **env_list);
void    execute_command(t_tree *root, char **env, t_env **env_list);
int		builtin_echo(t_tree *root);
int	cd_change_working_directory(t_tree *root, t_env **env_list);
int handle_shlvl(char *argv0, t_env **env_list);
int pwd_print_working_directory(t_tree *root, t_env **env_list);
int is_builtin(char *command);
int execute_builtin(t_tree *root, char **env, t_env **env_list);
int	env_environment(t_tree *root, char **env, t_env *env_list);
char *get_binary_file_path(t_tree *root, char **env, t_env **env_list);
int	exit_exe(t_tree *root);
int	export_command_builtin(t_tree *root, t_env **env_list);
int	builtin_unset_environment(t_tree *root, t_env **env_list, char **env);
void execute_command(t_tree *root, char **env, t_env **env_list);
void free_tree_exe(t_tree *root);
int exec_pipe(t_tree *root, char **env, int input_fd, t_env **env_list);
int execution(t_tree *root, char **env, t_env **env_list);
int setup_redirections(t_tree *root);
void free_tree_exe(t_tree *root);
void write_heredoc(int fd, const char *limiter,  t_env **env_list);
void create_heredoc(t_red *redir,  t_env **env_list);
void prepare_heredocs(t_tree *root,  t_env **env_list);
void apply_redirections(t_red *redir,  t_env **env_list);
int handle_heredocs(t_tree *root);
int setup_here_doc(t_tree *root);
int setup_input_for_read(t_tree *root);
int setup_input_for_append(t_tree *root);
int setup_input_for_trunk(t_tree *root);
int setup_redirections(t_tree *root);
void	print_debu(char *target, int time_to_sleep);
#endif

