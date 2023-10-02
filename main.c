/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maggie <maggie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2023/10/02 13:28:26 by maggie           ###   ########.fr       */
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
	}
	ft_free_strarray(paths);
	free (cmd);
	free (final_path);
	return (NULL);
}

int	executing_commands(char *cmd, int input_file, int output_file, char *envp[])
{
	int	pipefd[2];
	int	pid;
	char *output;

	
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*path;
	int		input_file;
	int		output_file;
	int		i;

	if (argc < 5)
		return (1);
	input_file = open(argv[1], O_RDONLY);
	output_file = open(argv[argc - 1], O_WRONLY);
	i = 2;
	while (i < argc - 1)
	{
		path = find_cmd_path(envp, argv[i++]);
		if (!path)
		{
			perror(path);
			return (1);
		}
		executing_commands(path, input_file, output_file, envp);
	}

	


}