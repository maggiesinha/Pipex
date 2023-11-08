/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maggie <maggie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2023/11/08 11:23:18 by maggie           ###   ########.fr       */
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

int	executing_commands(char *path, int input_file, int output_file, char *envp[], char *argv[])
{
	int	childpid;
	int	wait_status;
	char *cmd[2];

	cmd[0] = argv[2];
	cmd[1] = NULL;
	
	if (dup2(input_file, STDIN_FILENO) == -1) {
        perror("dup2 (input_file)");
        exit(1);
    }
	if (dup2(output_file, STDOUT_FILENO) == -1) {
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
		execve(path, cmd, envp);
	else
	{
		waitpid(childpid, &wait_status, 0);
		printf("Parent process runs after fork\n");
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*path;
	int		input_file;
	int		output_file;
	int		i;

	if (argc < 4)
		return (1);
	input_file = open(argv[1], O_RDONLY);
	output_file = open(argv[argc - 1], O_WRONLY);
	i = 2;
	while (i < argc - 1)
	{
		path = find_cmd_path(envp, argv[i]);
		if (!path)
		{
			perror(argv[i]);
			return (1);
		}
		executing_commands(path, input_file, output_file, envp, argv);
		i++;
	}

}