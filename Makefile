# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maggie <maggie@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/31 17:22:06 by mvalerio          #+#    #+#              #
#    Updated: 2023/11/18 13:06:21 by maggie           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = cc

CFLAGS = -Wall -Wextra -Werror -g

LIB = libft/libft.a

SRC = main.c command_path.c

OBJS = $(SRC:.c=.o)

BONUS_SRC = main_bonus.c command_path.c

BONUS_OBJS = $(BONUS_SRC:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

all: $(NAME)

$(NAME): $(OBJS)
	@make -s -C libft
	@make -s bonus -C libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB)
	@echo "Object files created. Pipex is now ready to run!"

clean:
	@rm -rf $(OBJS)
	@make -s clean -C libft
	@echo "Your object files were deleted."

fclean: clean
	@rm -rf $(NAME)
	@make -s fclean -C libft
	@echo "Your programs were deleted."

re: fclean all

bonus:
	make fclean
	@make -s -C libft
	@make -s bonus -C libft
	@$(CC) $(CFLAGS) -o $(NAME) $(BONUS_OBJS) $(LIB)
	@echo "Object files created. Pipex (with bonus) is now ready to run!"
