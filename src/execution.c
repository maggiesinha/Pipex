/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/26 23:19:53 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

// Prints the error message sent to it as an argument and exits the program.
void	ft_cmd_to_fd_error(int *in_out_fd, int *fds_to_close, char *str)
{
	if (in_out_fd[0] != -1)
		close(in_out_fd[0]);
	if (in_out_fd[1] != -1)
		close(in_out_fd[1]);
	if (fds_to_close[0] != -1)
		close(fds_to_close[0]);
	if (fds_to_close[1] != -1)
		close(fds_to_close[1]);
	perror(str);
	exit(1);
}

void	ft_cmd_to_fd_child(int *in_out_fd, int *fds_to_close, \
		t_args *curr, char *envp[])
{
	if (dup2(in_out_fd[0], STDIN_FILENO) == -1)
		ft_cmd_to_fd_error(in_out_fd, fds_to_close, \
		"dup2 (input_file)");
	if (dup2(in_out_fd[1], STDOUT_FILENO) == -1)
		ft_cmd_to_fd_error(in_out_fd, fds_to_close, \
		"dup2 (output_file)");
	if (fds_to_close[0] != -1)
		close(fds_to_close[0]);
	if (fds_to_close[1] != -1)
		close(fds_to_close[1]);
	if (in_out_fd[0] != -1)
		close(in_out_fd[0]);
	if (in_out_fd[1] != -1)
		close(in_out_fd[1]);
	execve(curr->path, curr->cmd_and_flags, envp);
}

// Executes the command in the child process. The parent process waits
// for the child process to finish (waitpid) and then returns, so that the
// program won't stop after executing the first command.
void	cmd_to_fd(int *in_out_fd, t_args *curr, char *envp[], int *fds_to_close)
{
	curr->pid = -5;
	if (!curr->skip_command)
	{
		curr->pid = fork();
		if (curr->pid == -1)
			ft_cmd_to_fd_error(in_out_fd, fds_to_close, "fork");
		if (curr->pid == 0)
		{
			ft_cmd_to_fd_child(in_out_fd, fds_to_close, curr, envp);
		}
		else
		{
			if (in_out_fd[0] != -1)
				close(in_out_fd[0]);
			if (in_out_fd[1] != -1)
				close(in_out_fd[1]);
		}
	}
	if (in_out_fd[0] != -1)
		close(in_out_fd[0]);
	if (in_out_fd[1] != -1)
		close(in_out_fd[1]);
}

void	ft_pipe_failed(int *pipefd, int tempfd)
{
	if (tempfd != -1)
		close(tempfd);
	if (pipefd[R] != -1)
		close(pipefd[R]);
	if (pipefd[W] != -1)
		close(pipefd[W]);
	exit(1);
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
			ft_pipe_failed(pipefd, tempfd);
		fds_to_close[0] = tempfd;
		fds_to_close[1] = pipefd[R];
		in_out_fd[0] = tempfd;
		in_out_fd[1] = pipefd[W];
		if (!current->skip_command)
			cmd_to_fd(in_out_fd, current, envp, fds_to_close);
		if (pipefd[W] != -1)
			close(pipefd[W]);
		if (tempfd != -1)
			close(tempfd);
		current = current->next;
	}
	return (current);
}
