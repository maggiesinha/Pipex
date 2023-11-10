/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maggie <maggie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:39:23 by mvalerio          #+#    #+#             */
/*   Updated: 2023/11/10 11:27:35 by maggie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include "libft/libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define W 1
#define R 0

typedef struct arguments {
	struct arguments	*head;
	struct arguments	*next;
	char				*path;
	char				**cmd_and_flags;
}				t_args;

char	**path_list(char *envp[]);
void	ft_error(char *str);
int		executing_command_to_fd(int input_fd, int output_fd, t_args *arg, char *envp[]);
void	ft_free_args(t_args *args);
void	ft_invalid_path(t_args *args);
void	ft_add_argument_to_list(t_args *current, t_args *args);
t_args	*ft_set_arguments(int argc, char *argv[], char *envp[]);
void	ft_execute_commands(t_args *args, int input_file, int output_file, char *envp[]);



#endif
