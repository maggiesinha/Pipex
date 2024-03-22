/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/22 12:54:08 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

// Executes the commands in the linked list (args), from the input file to
//the output file.
void	ft_exec_cmds(t_args *args, int in_fd, int out_fd, char *envp[])
{
	t_args	*current;
	int		pipefd[2];
	int		cmd_qty;
	int		*command_pids;

	cmd_qty = 0;
	command_pids = NULL;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		close(in_fd);
		close(out_fd);
		exit(1);
	}
	current = args->head;
	cmd_to_fd(in_fd, pipefd[W], current, envp, out_fd, pipefd[R]);
	close(pipefd[W]);
	current = current->next;
	command_pids = ft_execute_each_cmd(current, pipefd, envp, &cmd_qty);
	wait(NULL);
	cmd_to_fd(pipefd[R], out_fd, current, envp, in_fd, pipefd[W]);
	close(pipefd[R]);
	close(out_fd);
	ft_free_args(args);
}
