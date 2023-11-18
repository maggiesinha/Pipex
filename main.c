/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maggie <maggie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2023/11/18 12:55:57 by maggie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Prints the error message sent to it as an argument and exits the program.
void	ft_error(char *str)
{
	perror(str);
	exit(1);
}

// Executes the command in the child process. The parent process waits
// for the child process to finish (waitpid) and then returns, so that the
// program won't stop after executing the first command.
int	cmd_to_fd(int input_fd, int output_fd, t_args *arg, char *envp[])
{
	int		childpid;
	int		wait_status;

	if (dup2(input_fd, STDIN_FILENO) == -1)
		ft_error("dup2 (input_file)");
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		ft_error("dup2 (output_file)");
	childpid = fork();
	if (childpid == -1)
		ft_error("fork");
	if (childpid == 0)
	{
		close(input_fd);
		close(output_fd);
		execve(arg->path, arg->cmd_and_flags, envp);
	}
	else
	{
		close(input_fd);
		close(output_fd);
		waitpid(childpid, &wait_status, 0);
	}
	return (0);
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

// Executes the commands in the linked list (args), from the input file to 
//the output file.
void	ft_exec_cmds(t_args *args, int in_fd, int out_fd, char *envp[])
{
	t_args	*current;
	int		pipefd[2];
	int		tempfd;

	pipe(pipefd);
	current = args->head;
	cmd_to_fd(in_fd, pipefd[W], current, envp);
	close(pipefd[W]);
	current = current->next;
	while (current->next)
	{
		tempfd = pipefd[R];
		pipe(pipefd);
		cmd_to_fd(tempfd, pipefd[W], current->next, envp);
		close(pipefd[W]);
		close(tempfd);
		current = current->next;
	}
	cmd_to_fd(pipefd[R], out_fd, current, envp);
	close(pipefd[R]);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_args	*args;
	int		input_file;
	int		output_file;

	if (argc != 5)
		return (1);
	input_file = open(argv[1], O_RDONLY);
	output_file = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	args = ft_set_arguments(argc, argv, envp);
	ft_invalid_path(args);
	ft_exec_cmds(args, input_file, output_file, envp);
}
