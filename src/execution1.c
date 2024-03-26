/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/26 16:37:21 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

void	ft_exec_cmds_error(int in_fd, int out_fd)
{
	perror("pipe");
	close(in_fd);
	close(out_fd);
	exit(1);
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
	close(pipefd[W]);
	current = current->next;
	current = ft_execute_each_cmd(current, pipefd, envp);
	wait(NULL);
	fds_to_close[0] = in_fd;
	fds_to_close[1] = pipefd[W];
	in_out_fd[0] = pipefd[R];
	in_out_fd[1] = out_fd;
	cmd_to_fd(in_out_fd, current, envp, fds_to_close);
	close(pipefd[R]);
	close(out_fd);
	ft_free_args(args);
}
