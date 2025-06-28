// #include "../include/execution.h"

int exec_tree(t_tree *root, char **env, t_env **env_list, int input_fd, int in_subshell) 
{
	if (!root) 
	{
		printf("here for checkout if has null\n\n");
		sleep(10);
		return 1;
	}
	if (root->data == NULL && root->type == BLOCK && root->left) //then if the root->right == NULL
	{

		// this is a subshell (e.g., due to parentheses)
		pid_t pid = fork();
		//todo should check if fork return - 1; 
		// todo free all resources then exit
		if (pid == 0) 
		{
			// execute the inner subtree in child process
			if (expand_redir(root->redirections, *env_list) == R_FAIL)
				return (1);
			apply_redirections(root->redirections, env_list);
			exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
			exit(EXIT_SUCCESS);
		} 
		else 
		{
			// wait for child
			int status;
			waitpid(pid, &status, 0);
		//	printf("here int the exec_tree parentheses [%d] fucntion for check if found leaks \n", status);
		//	sleep(6);
			return WEXITSTATUS(status);
		}
	}
	if (root->type == BLOCK) 
	{
		//sleep(2);
		printf("here in exec tree for one command \n ");
		if (root->data)
		{
			//todo
			if (expand(&root->data, *env_list) == R_FAIL)
				return(1);
			// if (wildcard(&root->data) == R_FAIL)
			// 	return(1);
		}
		//todo check here the expand fucntion return int 
		if (in_subshell)
		{
			//printf("int subshell here 2\n\n\n");
			
			if (input_fd != STDIN_FILENO)
			{
					dup2(input_fd, STDIN_FILENO);
					close(input_fd);
			}
			run_command(root, env, env_list); // or execute_external_command
			exit(EXIT_FAILURE); // if it fails

		/* 	//todo Always fork for commands in subshells (e.g., pipelines)
			pid_t pid = fork();
			//todo should check if fork return - 1; 
		// todo free all resources then exit
			if (pid < 0)
			{
				perror("fork");
				return 1;
			}
			if (pid == 0) 
			{
				if (input_fd != STDIN_FILENO)
				{
					dup2(input_fd, STDIN_FILENO);
					close(input_fd);
				}
				run_command(root, env, env_list);
				exit(EXIT_FAILURE); //todo Unreachable
			}
			if (pid > 0)
			{
				int status;
				if (input_fd != STDIN_FILENO)
				{
					close(input_fd);
				}
				waitpid(pid, &status, 0);
				printf("here int the exec_tree block in subshell [%d] fucntion for check if found leaks \n", status);
				//sleep(6);
				if (WIFEXITED(status))
				{
					return WEXITSTATUS(status);
				}
				return 1;
			} */
		}
		else
		{
			//todo Not in subshell: execute built-ins directly, fork for external commands
			//sleep(2);
			//printf("in the exec tree for check why the program exit and the entered is valide command\n ");
			
			//printf("here 10000000\n\n\n");
			if (root->data != NULL &&  is_builtin(root->data[0]) == 0)
			{
				//sleep(2);
				printf("does built-in fucntion \n ");
				if (expand_redir(root->redirections, *env_list) == R_FAIL)
					return (1);
				int saved_stdin = dup(STDIN_FILENO);
				int saved_stdout = dup(STDOUT_FILENO);
				apply_redirections(root->redirections, env_list);
				int built_in_status = execute_builtin(root, env, env_list);
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				return (built_in_status);

			}
			pid_t pid = fork();
			//sleep(2);
			//printf("for pid-----------------------[%d] \n ", pid);
			if (pid < 0) 
			{
				perror("fork");
				return 1;
			}
			if (pid == 0) 
			{
				if (expand_redir(root->redirections, *env_list) == R_FAIL)
					return (1); //todo here you in the child process should use exit not return because error
				if (apply_redirections(root->redirections, env_list) == 1)
				{
					exit(1);
					return 1; // 
				}
				if (input_fd != STDIN_FILENO)
				{
					printf("FOR CHECK THE FILE DIRCRIPTE HERE ONE FUCK OFF \n ");
					dup2(input_fd, STDIN_FILENO);
					close(input_fd);
				}
				// sleep(2);
				 printf("open the execute external command \n ");
				 execute_external_command(root, env_list);
				// sleep(2);
				printf("here for  check why it exit and should not exit\n ");
				//exit(EXIT_FAILURE); //todo Unreachable
			}
			if (pid > 0) 
			{
				int status;
				if (input_fd != STDIN_FILENO)
				{
					close(input_fd);
				}
				waitpid(pid, &status, 0);
				printf("here int the exec_tree none subshell [%d] fucntion for check if found leaks \n", status);
			//	sleep(6);
				if (WIFEXITED(status))
				{
					printf("WIFEXITED [%d][%d] fucntion for check if found leaks \n", status, WEXITSTATUS(status));
					//sleep(6);
					return WEXITSTATUS(status);
				}
				return 1;
			}
		}
	}
	if (root->type == PIPE)
	{
		//printf("in execution pipe inside tree_exe\n\n");
		return exec_pipe(root, env, input_fd, env_list);
	}
	if (root->type == OP_AND)
	{
		//printf("in execution && inside tree_exe\n\n");
		int left_status = exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
		if (left_status == 0)
		{
			return exec_tree(root->right, env, env_list, STDIN_FILENO, 0);
		}
		return left_status;
	}
	if (root->type == OP_OR)
	{
	//	printf("in execution || inside tree_exe\n\n");
		int left_status = exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
		if (left_status != 0) 
		{
			return exec_tree(root->right, env, env_list, STDIN_FILENO, 0);
		}
		return left_status;
	}
	return 1;
}

