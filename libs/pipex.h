/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:39:23 by mvalerio          #+#    #+#             */
/*   Updated: 2024/03/27 17:02:55 by mvalerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libs/libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include "../libs/libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

# define W 1
# define R 0

typedef struct arguments {
	struct arguments	*head;
	struct arguments	*next;
	char				*path;
	char				**cmd_and_flags;
	int					infile_invalid;
	int					skip_command;
	int					pid;
}				t_args;

char	*ft_absolute_path(char *cmd);
char	**path_list(char *envp[]);
void	ft_error(char *str);
void	cmd_to_fd(int *in_out_fd, t_args *curr, \
		char *envp[], int *fds_to_close);
void	ft_free_args(t_args *args);
void	ft_invalid_path(t_args *args);
void	ft_add_argument_to_list(t_args *current, t_args *args);
void	ft_set_arguments(t_args *args, int argc, char *argv[], char *envp[]);
t_args	*ft_execute_each_cmd(t_args *current, int pipefd[2], char *envp[]);
void	ft_exec_cmds(t_args *args, int in_fd, int out_fd, char *envp[]);
void	ft_add_child_pid(int pid, int *cmd_pids, int *cmd_qty);
void	ft_cmd_to_fd_child(int *in_out_fd, int *fds_to_close, \
		t_args *curr, char *envp[]);
void	ft_pipe_failed(int *pipefd, int tempfd);
void	ft_end_of_execution(t_args *args, int *pipefd, int *out_fd);
void	ft_no_outfile(char *argv[], int argc, int input_file, t_args *args);
char	**ft_error_handling(int argc, char *envp[], int main_or_bonus);

#endif
