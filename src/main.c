/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/26 23:34:55 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_args	*args;
	int		input_file;
	int		output_file;

	envp = ft_error_handling(argc, envp, 0);
	args = malloc(sizeof(t_args));
	if (!args)
		return (1);
	args->infile_invalid = 0;
	input_file = open(argv[1], O_RDONLY);
	if (input_file == -1)
	{
		ft_printf("%s: No such file or directory\n", argv[1]);
		args->infile_invalid = 1;
	}
	output_file = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (output_file == -1)
		ft_no_outfile(argv, argc, input_file, args);
	ft_set_arguments(args, argc, argv, envp);
	ft_invalid_path(args);
	ft_exec_cmds(args, input_file, output_file, envp);
}
