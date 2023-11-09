/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2023/11/09 13:29:59 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Looks for the PATH variable in the environment variables and returns all
//the paths in an array of strings.
char	**path_list(char *envp[])
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH", 5))
			break ;
		i++;
	}
	if (!envp[i])
		return (NULL);
	paths = ft_split(&(envp[i][5]), ':');
	return (paths);
}

// Finds the path to the command in the PATH variable. If the command is not
//found, it returns NULL.
char	*find_cmd_path(char *envp[], char *cmd)
{
	char	**paths;
	char	*final_path;
	int		i;

	cmd = ft_strjoin("/", cmd);
	paths = path_list(envp);
	i = 0;
	while (paths[i])
	{
		final_path = ft_strjoin(paths[i], cmd);
		if (access(final_path, X_OK) == 0)
		{
			ft_free_strarray(paths);
			free (cmd);
			return (final_path);
		}
		i++;
		free(final_path);
	}
	ft_free_strarray(paths);
	free (cmd);
	return (NULL);
}

void	ft_error(char *str)
{
	perror(str);
	exit(1);
}

// Executes the command in the child process. The parent process waits
// for the child process to finish (waitpid) and then returns, so that the
// program won't stop after executing the first command.
int	executing_command_to_fd(int input_fd, int output_fd, t_args *arg, char *envp[])
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

// Checks if the path to the command is valid. If not, it frees the linked
//list (args) and exits the program with an error message.
void	ft_invalid_path(t_args *args)
{
	t_args	*current;

	current = args->head;
	while (current)
	{
		if (!(current->path))
		{
			perror(current->cmd_and_flags[0]);
			ft_free_args(args);
			exit(1);
		}
		current = current->next;
	}
}

// Adds each new argument to the linked list of commands (args).
void	ft_add_argument_to_list(t_args *current, t_args *args)
{
	t_args	*temp;

	if (!(args->head))
		args->head = current;
	else
	{
		temp = args->head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = current;
	}
}

// Sets a linked list (args) of all the commands that the program is going
//to execute. In this list, there is the path to the command ,a string), and
// the command and flags, an array of strings.
t_args	*ft_set_arguments(int argc, char *argv[], char *envp[])
{
	t_args	*args;
	t_args	*current;
	int		i;

	i = 2;
	args = malloc(sizeof(t_args));
	if (!args)
		return (NULL);
	while (i < argc - 1)
	{
		current = malloc(sizeof(t_args));
		if (!current)
			return (NULL);
		current->next = NULL;
		current->cmd_and_flags = ft_split(argv[i], ' ');
		current->path = find_cmd_path(envp, current->cmd_and_flags[0]);
		ft_add_argument_to_list(current, args);
		i++;
	}
	return (args);
}

// Executes the commands in the linked list (args), from the
void	ft_execute_commands(t_args *args, int input_file, int output_file, char *envp[])
{
	t_args	*current;
	int		pipefd[2];

	pipe(pipefd);
	current = args->head;
	executing_command_to_fd(input_file, pipefd[W],current, envp);
	close(pipefd[W]);
	current = current->next;
	executing_command_to_fd(pipefd[R], output_file, current, envp);
	close(pipefd[R]);

}

int	main(int argc, char *argv[], char *envp[])
{
	t_args	*args;
	int		input_file;
	int		output_file;

	if (argc < 5)
		return (1);
	input_file = open(argv[1], O_RDONLY);
	output_file = open(argv[argc - 1], O_WRONLY);
	args = ft_set_arguments(argc, argv, envp);
	ft_invalid_path(args);
	ft_execute_commands(args, input_file, output_file, envp);
}
