/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2023/11/21 12:46:02 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_args	*args;
	int		input_file;
	int		output_file;

	if (argc != 5)
	{
		ft_printf("Error: incorrect number of arguments\n");
		return (1);
	}
	input_file = open(argv[1], O_RDONLY);
	output_file = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	args = ft_set_arguments(argc, argv, envp);
	ft_invalid_path(args);
	ft_exec_cmds(args, input_file, output_file, envp);
}
