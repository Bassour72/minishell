/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 18:58:41 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/09 14:03:28 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H
# include "parsing.h"
# include <sys/wait.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/stat.h>
# include <errno.h>
# include <stdbool.h>
# define ERR_DOT_DOT_DOT        ": invalid argument (three consecutive dots)\n"
# define ERR_HYPHEN_HYPHEN_HYPHEN ": invalid argument \
(three consecutive hyphens)\n"
# define ERR_DOT_HYPHEN         ": invalid argument (\".-\" not allowed)\n"
# define ERR_HYPHEN_DOT         ": invalid argument (\"-.\" not allowed)\n"
# define ERR_HYPHEN_SLASH       ": invalid argument (\"-/\" not allowed)\n"
# define ERR_GETCWD_FAIL        "cd: error retrieving current directory: getcwd: \
cannot access parent directories"
# define DOT_USAGE_ERROR ".: filename argument required.\n.: \
usage: . filename [arguments]\n"
# define MAX_HEREDOC 16
# define EXIT_MALLOC_FAIL -1
# define STATUS_OK 0
# define STATUS_ERROR 1
# define STATUS_NOT_FOUND 127
# define STATUS_IS_DIR 126
# define CMD_CANNOT_EXECUTE  126
# define CMD_NOT_FOUND 127
# define EX_USAGE 2

extern int	g_exit_status;
char	*get_env_path_parent(const char *path);
int		check_argument(t_tree *root);
int		recover_invalid_pwd(t_env **env, char *new_pwd);
void	check_non_interactive_exit(t_tree *root, t_env **env_list, \
		int exit_status, bool should_free);
int		execution(t_tree *root, t_env **env_list);
void	execute_command(t_tree *root, t_env **env_list);
int		builtin_echo(t_tree *root);
int		cd_change_working_directory(t_tree *root, t_env **env_list);
int		handle_shlvl(char *argv0, t_env **env_list);
int		pwd_print_working_directory(t_tree *root, t_env **env_list);
int		is_builtin(char *command);
int		execute_builtin(t_tree *root, t_env **env_list);
int		env_environment(t_tree *root, t_env *env_list);
char	*get_binary_file_path(t_tree *root, t_env **env_list);
int		exit_exe(t_tree *root, t_env **env_list);
int		export_command_builtin(t_tree *root, t_env **env_list);
int		builtin_unset_environment(t_tree *root, t_env **env_list);
void	execute_command(t_tree *root, t_env **env_list);
int		exec_pipe(t_tree *root, t_env **env_list, bool is_child);
int		execution(t_tree *root, t_env **env_list);
int		setup_redirections(t_tree *root);
int		write_heredoc(int fd, const char *limiter);
int		create_heredoc(t_red *redir);
int		prepare_heredocs(t_tree *root, t_env **env_list);
int		apply_redirections(t_red *redir, t_env **env_list);
int		handle_heredocs(t_tree *root);
int		setup_here_doc(t_tree *root);
int		setup_input_for_read(t_tree *root);
int		setup_input_for_append(t_tree *root);
int		setup_input_for_trunk(t_tree *root);
int		setup_redirections(t_tree *root);
void	print_debu(char *target, int time_to_sleep);
int		handle_shlvl(char *argv0, t_env **env_list);
void	run_command(t_tree *root, t_env **env_list);
void	execute_external_command(t_tree *root, t_env **env_list);
char	**gen_new_env(t_env *env_list);
int		is_illegal_cd_arg(const char *arg);
void	update_pwd(t_env **env);
void	set_env_var(char *key, char *value, t_env **env);
char	*get_arg_cd(const char *arg);
char	*get_env_value(char *key, t_env *env);
int		diagnose_cd_error(const char *path, int print_error);
int		exec_tree(t_tree *root, t_env **env_list, \
		int in_subshell, bool is_child);
int		should_display_error(char *cmd, t_env **env_list, bool should_print);
int		display_error(char *sms_error, const char *target, int return_error);
int		cd_dotdots_only(t_env **env, char *arg);
void	close_pipe_fds(int pipefd[2]);
int		is_fd_open(int fd);
void	close_all_fds_(void);
int		create_pipe(int pipefd[2], t_tree *root, \
		t_env **env_list, bool is_child);
void	update_pwd(t_env **env);
char	*get_env_value(char *key, t_env *env);
void	update_existing_env(t_env *existing, char *new_value, int append_mode);
t_env	*is_exist_env(t_env *env_list, const char *new_key);
void	list_env_add_back(t_env **env_list, t_env *new_node_env);
t_env	*copy_env_list(t_env *env_list);
t_env	*sort_env_list(t_env *env_list);
void	print_env_export_sort(t_env *env_list, t_tree *root);
int		ft_strcmp_v2(const char *s1, const char *s2);
int		is_valid_identifier(const char *identifier, int *is_append_mode);
char	*get_env_key(const char *identifier);
char	*get_env_value1(const char *identifier);
void	swap_node(t_env *a, t_env *b);
int		is_valid_shlvl_string(char *str);
///
void	close_heredoc_fds(t_tree *root);
void	propagate_fork_flag(t_tree *root, int is_forked);
int		count_heredocs(t_tree *node);
void	enforce_heredoc_limit(t_tree *root, t_env **env_list);
char	*extract_path_variable(t_env **env_list);
char	*check_valid_command_path(char *command);
int		wait_child_status(pid_t pid);
int		exec_subshell(t_tree *root, t_env **env_list);
int		exec_builtin_command(t_tree *root, t_env **env_list);
int		exec_external_command(t_tree *root, t_env **env_list);
int		handle_cd_failure(t_env **env, char *arg, char *logical_pwd);
int		free_all(char *a, char *b, char *c);
char	*build_logical_path(char *base, char *suffix);
int		apply_single_redirection(t_red *redir, t_env **env_list);
long	ft_atol(const char *str);
#endif