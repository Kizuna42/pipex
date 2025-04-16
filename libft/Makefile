# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/25 16:01:19 by kizuna            #+#    #+#              #
#    Updated: 2025/04/16 01:05:43 by kizuna           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror -I. -Isrc/gnl -Isrc/printf -Isrc/printf/include

SRCS_DIR = src/
FT_IS_DIR = $(SRCS_DIR)ft_is/
FT_LST_DIR = $(SRCS_DIR)ft_lst/
FT_MEM_DIR = $(SRCS_DIR)ft_mem/
FT_PUT_DIR = $(SRCS_DIR)ft_put/
FT_STR_DIR = $(SRCS_DIR)ft_str/
FT_TO_DIR = $(SRCS_DIR)ft_to/
GNL_DIR = $(SRCS_DIR)gnl/
PRINTF_DIR = $(SRCS_DIR)printf/srcs/

SRCS = $(FT_IS_DIR)ft_isalnum.c \
		$(FT_IS_DIR)ft_isalpha.c \
		$(FT_IS_DIR)ft_isascii.c \
		$(FT_IS_DIR)ft_isdigit.c \
		$(FT_IS_DIR)ft_isprint.c \
		$(FT_TO_DIR)ft_atoi.c \
		$(FT_TO_DIR)ft_itoa.c \
		$(FT_TO_DIR)ft_tolower.c \
		$(FT_TO_DIR)ft_toupper.c \
		$(FT_TO_DIR)ft_abs.c \
		$(FT_MEM_DIR)ft_bzero.c \
		$(FT_MEM_DIR)ft_calloc.c \
		$(FT_MEM_DIR)ft_memchr.c \
		$(FT_MEM_DIR)ft_memcmp.c \
		$(FT_MEM_DIR)ft_memcpy.c \
		$(FT_MEM_DIR)ft_memmove.c \
		$(FT_MEM_DIR)ft_memset.c \
		$(FT_PUT_DIR)ft_putchar_fd.c \
		$(FT_PUT_DIR)ft_putendl_fd.c \
		$(FT_PUT_DIR)ft_putnbr_fd.c \
		$(FT_PUT_DIR)ft_putstr_fd.c \
		$(FT_PUT_DIR)ft_putstr.c \
		$(FT_PUT_DIR)ft_putnstr.c \
		$(FT_STR_DIR)ft_split.c \
		$(FT_STR_DIR)ft_split_mult.c \
		$(FT_STR_DIR)ft_strchr.c \
		$(FT_STR_DIR)ft_strdup.c \
		$(FT_STR_DIR)ft_strjoin.c \
		$(FT_STR_DIR)ft_strlcat.c \
		$(FT_STR_DIR)ft_strlcpy.c \
		$(FT_STR_DIR)ft_strlen.c \
		$(FT_STR_DIR)ft_strmapi.c \
		$(FT_STR_DIR)ft_strncmp.c \
		$(FT_STR_DIR)ft_strnstr.c \
		$(FT_STR_DIR)ft_strrchr.c \
		$(FT_STR_DIR)ft_strtrim.c \
		$(FT_STR_DIR)ft_substr.c \
		$(FT_STR_DIR)ft_striteri.c \
		$(FT_STR_DIR)ft_strnchr.c \
		$(GNL_DIR)get_next_line.c \
		$(GNL_DIR)get_next_line_utils.c \
		$(GNL_DIR)get_next_line_utils2.c

PRINTF_SRCS = $(PRINTF_DIR)ft_printf_utils.c \
		$(PRINTF_DIR)ft_print_unsigned_long_flag.c \
		$(PRINTF_DIR)ft_print_unsigned_long_utils.c \
		$(PRINTF_DIR)ft_printf.c \
		$(PRINTF_DIR)ft_printf_flag.c \
		$(PRINTF_DIR)ft_printf_flag_utils.c \
		$(PRINTF_DIR)ft_print_s_null.c \
		$(PRINTF_DIR)ft_print_unsigned.c \
		$(PRINTF_DIR)ft_print_unsigned_flag.c \
		$(PRINTF_DIR)ft_print_unsigned_long.c \
		$(PRINTF_DIR)ft_print_int_flag.c \
		$(PRINTF_DIR)ft_print_percent.c \
		$(PRINTF_DIR)ft_print_s.c \
		$(PRINTF_DIR)ft_print_c.c \
		$(PRINTF_DIR)ft_print_int.c \
		$(PRINTF_DIR)ft_initialize.c

BONUS = $(FT_LST_DIR)ft_lstnew_bonus.c \
		$(FT_LST_DIR)ft_lstadd_front_bonus.c \
		$(FT_LST_DIR)ft_lstsize_bonus.c \
		$(FT_LST_DIR)ft_lstlast_bonus.c \
		$(FT_LST_DIR)ft_lstadd_back_bonus.c \
		$(FT_LST_DIR)ft_lstdelone_bonus.c \
		$(FT_LST_DIR)ft_lstclear_bonus.c \
		$(FT_LST_DIR)ft_lstiter_bonus.c \
		$(FT_LST_DIR)ft_lstmap_bonus.c

# ヘッダーファイルの依存関係
INCLUDES = -I. -Isrc/gnl -Isrc/printf

OBJS = $(SRCS:.c=.o)
PRINTF_OBJS = $(PRINTF_SRCS:.c=.o)
BONUS_OBJS = $(BONUS:.c=.o)

ALL_SRCS = $(SRCS) $(PRINTF_SRCS) $(BONUS)
ALL_OBJS = $(ALL_SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(ALL_OBJS)
	ar rcs $(NAME) $(ALL_OBJS)

clean:
	rm -f $(ALL_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
