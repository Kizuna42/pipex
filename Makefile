# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 20:15:47 by kizuna            #+#    #+#              #
#    Updated: 2025/05/02 19:52:18 by kizuna           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CFLAGS = -Werror -Wall -Wextra -fsanitize=address

RM = rm -rf

SRCS = 	src/pipex.c\
		src/utils.c\
		src/path.c\
		libft/libft.a\

SRCS_BONUS = 	src_bonus/pipex_bonus.c\
				src_bonus/utils_bonus.c\
				src_bonus/path_bonus.c\
				libft/libft.a\

$(NAME) :
	make all -C libft
	cc $(CFLAGS) $(SRCS) -o $(NAME)


all : $(NAME)

fclean : clean
	$(RM) $(NAME)
	make fclean -C libft

clean :
	$(RM) $(NAME)
	make clean -C libft

re : fclean all

bonus : clean
	make all -C libft
	cc $(CFLAGS) $(SRCS_BONUS) -o $(NAME)

.PHONY : all clean fclean re bonus
