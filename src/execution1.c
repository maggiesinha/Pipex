/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/27 18:41:32 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

char	*ft_absolute_path(char *cmd)
{
	if (access(cmd, X_OK) == 0)
	{
		cmd = ft_strjoin("", cmd);
		return (cmd);
	}
	return (NULL);
}

void	ft_exec_cmds_error(int in_fd, int out_fd)
{
	perror("pipe");
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
	exit(1);
}

void	ft_end_of_execution(t_args *args, int *pipefd, int *in_out_fd)
{
	t_args	*temp;

	temp = args->head;
	while (temp)
	{
		if (temp->pid != -5)
			waitpid(temp->pid, NULL, 0);
		temp = temp->next;
	}
	if (pipefd[R] != -1)
		close(pipefd[R]);
	if (in_out_fd[1] != -1)
		close(in_out_fd[1]);
	ft_free_args(args);
}

// Executes the commands in the linked list (args), from the input file to
//the output file.
void	ft_exec_cmds(t_args *args, int in_fd, int out_fd, char *envp[])
{
	t_args	*current;
	int		pipefd[2];
	int		fds_to_close[2];
	int		in_out_fd[2];

	if (pipe(pipefd) == -1)
		ft_exec_cmds_error(in_fd, out_fd);
	current = args->head;
	fds_to_close[0] = out_fd;
	fds_to_close[1] = pipefd[R];
	in_out_fd[0] = in_fd;
	in_out_fd[1] = pipefd[W];
	cmd_to_fd(in_out_fd, current, envp, fds_to_close);
	if (pipefd[W] != -1)
		close(pipefd[W]);
	current = current->next;
	current = ft_execute_each_cmd(current, pipefd, envp);
	fds_to_close[0] = in_fd;
	fds_to_close[1] = pipefd[W];
	in_out_fd[0] = pipefd[R];
	in_out_fd[1] = out_fd;
	cmd_to_fd(in_out_fd, current, envp, fds_to_close);
	ft_end_of_execution(args, pipefd, in_out_fd);
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
