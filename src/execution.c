/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/22 12:52:43 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

// Adds the pid sent to the end of the cmd_pids array
void	ft_add_child_pid(int pid, int *cmd_pids, int *cmd_qty)
{
	int	*cmd_pids_final;
	int	i;

	(*cmd_qty)++;
	if (*cmd_qty == 1)
	{
		cmd_pids = malloc(sizeof(int));
		if (!cmd_pids)
			return ;
		cmd_pids[0] = pid;
		return ;
	}
	cmd_pids_final = malloc(sizeof(int) * (*cmd_qty));
	if (!cmd_pids)
		return ;
	i = 0;
	while (i < *cmd_qty)
		cmd_pids_final[i] = cmd_pids[i];
	cmd_pids_final[i] = pid;
	free(cmd_pids);
	cmd_pids = cmd_pids_final;
	return ;
}

// Prints the error message sent to it as an argument and exits the program.
void	ft_error(char *str)
{
	perror(str);
	exit(1);
}

// Executes the command in the child process. The parent process waits
// for the child process to finish (waitpid) and then returns, so that the
// program won't stop after executing the first command.
int	cmd_to_fd(int input_fd, int output_fd, t_args *arg, char *envp[], int to_close_fd1, int to_close_fd2)
{
	int		childpid;

	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		close(input_fd);
		close(output_fd);
		close(to_close_fd1);
		close(to_close_fd2);
		ft_error("dup2 (input_file)");
	}
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		close(input_fd);
		close(output_fd);
		close(to_close_fd1);
		close(to_close_fd2);
		ft_error("dup2 (output_file)");
	}
	childpid = fork();
	if (childpid == -1)
	{
		close(input_fd);
		close(output_fd);
		close(to_close_fd1);
		close(to_close_fd2);
		ft_error("fork");
	}
	if (childpid == 0)
	{

		execve(arg->path, arg->cmd_and_flags, envp);
	}
	else
	{
		close(input_fd);
		close(output_fd);
	}
	return (childpid);
}

// Frees the linked list (args).
void	ft_free_args(t_args *args)
{
	t_args	*current;
	t_args	*temp;

	current = args->head;
	while (current)
	{
		temp = current;
		current = current->next;
		ft_free_strarray(temp->cmd_and_flags);
		free(temp->path);
		free(temp);
	}
	free(args);
}

int	*ft_execute_each_cmd(t_args *current, int pipefd[2], \
	char *envp[], int *cmd_qty)
{
	int	tempfd;
	int	*command_pids;

	command_pids = NULL;
	while (current->next)
	{
		tempfd = pipefd[R];
		//close(pipefd[R]);
		if (pipe(pipefd) == -1){
			close(tempfd);
			close(pipefd[R]);
			close(pipefd[W]);
			exit(1);
		}
		ft_add_child_pid(cmd_to_fd(tempfd, pipefd[W], current->next, envp, tempfd, pipefd[R]),
			command_pids, cmd_qty);
		close(pipefd[W]);
		close(tempfd);
		current = current->next;
	}
	return (command_pids);
}
