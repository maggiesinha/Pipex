/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:39:23 by mvalerio          #+#    #+#             */
/*   Updated: 2023/11/09 13:15:33 by mvalerio         ###   ########.fr       */
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

#endif
