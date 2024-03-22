/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/22 12:45:09 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_args	*args;
	int		input_file;
	int		output_file;

	if (argc < 5)
	{
		ft_printf("Error: not enough arguments\n");
		return (1);
	}
	input_file = open(argv[1], O_RDONLY);
	if (input_file == -1)
	{
		ft_printf("%s: No such file or directory\n", argv[1]);
		return (1);
	}
	output_file = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (output_file == -1)
	{
		ft_printf("%s: Outfile open failed.\n", argv[1]);
		return (1);
	}
	args = ft_set_arguments(argc, argv, envp);
	ft_invalid_path(args, input_file, output_file);
	ft_exec_cmds(args, input_file, output_file, envp);
}
