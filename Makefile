# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#              #
#    Updated: 2025/04/16 17:13:02 by kizuna           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = src/main.c \
		src/pipex_utils.c \
		src/pipex_init.c \
		src/pipex_files.c \
		src/pipex_path.c \
		src/processes.c \
		src/pipex_pipes.c \
		src/pipex_heredoc.c \
		src/pipex_dir_utils.c

BONUS_SRCS = src/main.c \
		src/pipex_utils.c \
		src/pipex_files.c \
		src/pipex_path.c \
		src/processes.c \
		src/pipex_bonus_utils.c \
		src/pipex_bonus_heredoc.c \
		src/pipex_init_bonus.c \
		src/pipex_pipes.c \
		src/pipex_dir_utils.c

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

bonus: $(LIBFT) $(BONUS_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(BONUS_OBJS) -L$(LIBFT_DIR) -lft

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BONUS_OBJS) bonus
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus
