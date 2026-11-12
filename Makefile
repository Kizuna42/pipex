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

CFLAGS = -Wall -Wextra -Werror $(EXTRA_CFLAGS)
ASAN_FLAGS = -g -fsanitize=address
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

test: all bonus
	./tests/run_tests.sh

# AddressSanitizer 付きビルド（提出用の通常ビルドには含めない）
asan: fclean
	$(MAKE) all bonus EXTRA_CFLAGS="$(ASAN_FLAGS)"

asan_test: asan
	./tests/run_tests.sh

.PHONY: all clean fclean re bonus test asan asan_test
