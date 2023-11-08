/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maggie <maggie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2023/11/08 11:57:15 by maggie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft/libft.h"
#include <unistd.h>
#include <fcntl.h>

char	**path_list (char *envp[])
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH", 5))
			break;
		i++;
	}
	if (!envp[i])
		return (NULL);
	paths = ft_split(&(envp[i][5]), ':');
	return (paths);
}


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

int	executing_command_to_fd(char *path, int input_fd, int output_fd, char *envp[], char *argv[])
{
	int	childpid;
	int	wait_status;
	char *cmd[2];

	cmd[0] = argv[2];
	cmd[1] = NULL;
	
	if (dup2(input_fd, STDIN_FILENO) == -1) {
        perror("dup2 (input_file)");
        exit(1);
    }
	if (dup2(output_fd, STDOUT_FILENO) == -1) {
        perror("dup2 (output_file)");
        exit(1);
    }
	childpid = fork(); // Create a child process
    if (childpid == -1)
	{
        perror("fork");
        return 1;
    }
	if (childpid == 0)
	{
		close (input_fd);
		close(output_fd);
		execve(path, cmd, envp);
	}
	else
	{
		close (input_fd);
		close(output_fd);
		waitpid(childpid, &wait_status, 0);
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*path[2];
	int		input_file;
	int		output_file;
	int		pipefd[2];

	if (argc < 5)
		return (1);
	input_file = open(argv[1], O_RDONLY);
	output_file = open(argv[argc - 1], O_WRONLY);
	path[0] = find_cmd_path(envp, argv[2]);
	path[1] = find_cmd_path(envp, argv[3]);
	if (!(path[0] && path[1]))
	{
		free (path[0]);
		free (path[1]);
		perror(argv[2]);
		return (1);
	}
	pipe(pipefd);
	executing_command_to_fd(path[0], input_file, pipefd[1], envp, argv);
	close(pipefd[1]);
	executing_command_to_fd(path[1], pipefd[0], output_file, envp, argv);
	close(pipefd[0]);
}