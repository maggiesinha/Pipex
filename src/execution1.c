/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/25 16:11:29 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

// Executes the commands in the linked list (args), from the input file to
//the output file.
void	ft_exec_cmds(t_args *args, int in_fd, int out_fd, char *envp[])
{
	t_args	*current;
	int		pipefd[2];

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
	current = ft_execute_each_cmd(current, pipefd, envp);
	wait(NULL);
	cmd_to_fd(pipefd[R], out_fd, current, envp, in_fd, pipefd[W]);
	close(pipefd[R]);
	close(out_fd);
	ft_free_args(args);
}
