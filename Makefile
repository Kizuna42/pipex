# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 20:15:47 by kizuna            #+#    #+#              #
#    Updated: 2025/05/04 17:30:02 by kizuna           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
NAME_BONUS = pipex_bonus

CFLAGS = -Wall -Wextra -Werror -fsanitize=address
RM = rm -rf

SRCS = src/pipex.c src/utils.c
BONUS_SRCS = src/pipex_bonus.c src/utils_bonus.c src/utils.c

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	cc $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

bonus: $(LIBFT) $(BONUS_OBJS)
	cc $(CFLAGS) $(BONUS_OBJS) -L$(LIBFT_DIR) -lft -o $(NAME_BONUS)

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re bonus
