/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:59:53 by maggie            #+#    #+#             */
/*   Updated: 2024/03/26 23:35:18 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/pipex.h"

void	ft_no_outfile(char *argv[], int argc, int input_file, t_args *args)
{
	ft_printf("%s: Outfile open failed.\n", argv[argc - 1]);
	free(args);
	if (input_file != -1)
		close(input_file);
	exit (1);
}

char	**ft_error_handling(int argc, char *envp[], int main_or_bonus)
{
	int	flag;

	if (main_or_bonus == 0)
		flag = (argc != 5);
	else
		flag = (argc < 5);
	if (flag)
	{
		ft_printf("Error: incorrect number of arguments\n");
		exit (1);
	}
	if (!(*envp))
	{
		envp = malloc(sizeof(char *) * 2);
		if (!envp)
			exit(1);
		envp[0] = ft_strjoin("PATH=/usr/bin", "");
		envp[1] = NULL;
		return (envp);
	}
	return (envp);
}
