# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvalerio <mvalerio@student.42lisboa.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/31 17:22:06 by mvalerio          #+#    #+#              #
#    Updated: 2024/03/26 23:37:11 by mvalerio         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

NAME_BONUS = pipex_bonus

CC = cc

CFLAGS = -Wall -Wextra -Werror -g

LIBFT = libs/libft

LIB_A = libs/libft/libft.a

SRC = src/command_path.c src/execution.c src/execution1.c src/extra.c

OBJS = $(SRC:.c=.o)

MAIN = src/main.c

MAIN_OBJS = $(MAIN:.c=.o)

MAIN_BONUS = src/main_bonus.c

MAIN_BONUS_OBJS = $(MAIN_BONUS:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

all: $(NAME)

$(NAME): $(OBJS) $(MAIN_OBJS)
	@make -s -C $(LIBFT)
	@make -s bonus -C $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(MAIN_OBJS) $(OBJS) $(LIB_A)
	@echo "Object files created. Pipex is now ready to run!"

clean:
	@rm -rf $(OBJS) $(MAIN_OBJS) $(MAIN_BONUS_OBJS)
	@make -s clean -C $(LIBFT)
	@echo "Your object files were deleted."

fclean: clean
	@rm -rf $(NAME) $(NAME_BONUS)
	@make -s fclean -C $(LIBFT)
	@echo "Your programs were deleted."

re: fclean all

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS) $(MAIN_BONUS_OBJS)
	@$(CC) $(CFLAGS) -o $(NAME_BONUS) $(MAIN_BONUS_OBJS) $(OBJS) $(LIB_A)
	@echo "Object files created. Pipex (with bonus) is now ready to run!"
