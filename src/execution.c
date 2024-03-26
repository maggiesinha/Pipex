/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/26 17:00:17 by mvalerio         ###   ########.fr       */
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
void	ft_cmd_to_fd_error(int *in_out_fd, int *fds_to_close, char *str)
{
	close(in_out_fd[0]);
	close(in_out_fd[1]);
	close(fds_to_close[0]);
	close(fds_to_close[1]);
	perror(str);
	exit(1);
}

// Executes the command in the child process. The parent process waits
// for the child process to finish (waitpid) and then returns, so that the
// program won't stop after executing the first command.
int	cmd_to_fd(int *in_out_fd, t_args *arg, char *envp[], int *fds_to_close)
{
	int		childpid;

	childpid = fork();
	if (childpid == -1)
		ft_cmd_to_fd_error(in_out_fd, fds_to_close, "fork");
	if (childpid == 0)
	{
		if (dup2(in_out_fd[0], STDIN_FILENO) == -1)
			ft_cmd_to_fd_error(in_out_fd, fds_to_close, "dup2 (input_file)");
		if (dup2(in_out_fd[1], STDOUT_FILENO) == -1)
			ft_cmd_to_fd_error(in_out_fd, fds_to_close, "dup2 (output_file)");
		close(fds_to_close[0]);
		close(fds_to_close[1]);
		execve(arg->path, arg->cmd_and_flags, envp);
	}
	else
	{
		close(in_out_fd[0]);
		close(in_out_fd[1]);
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

t_args	*ft_execute_each_cmd(t_args *current, int pipefd[2], \
	char *envp[])
{
	int	tempfd;
	int	fds_to_close[2];
	int	in_out_fd[2];

	while (current->next)
	{
		tempfd = pipefd[R];
		if (pipe(pipefd) == -1)
		{
			close(tempfd);
			close(pipefd[R]);
			close(pipefd[W]);
			exit(1);
		}
		fds_to_close[0] = tempfd;
		fds_to_close[1] = pipefd[R];
		in_out_fd[0] = tempfd;
		in_out_fd[1] = pipefd[W];
		cmd_to_fd(in_out_fd, current, envp, fds_to_close);
		close(pipefd[W]);
		close(tempfd);
		current = current->next;
	}
	return (current);
}
