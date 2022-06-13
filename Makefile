# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/04 13:33:13 by bsavinel          #+#    #+#              #
#    Updated: 2022/06/13 16:41:14 by bsavinel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
########						Comilation Tools						########
################################################################################

NAME = miniRT

CC = cc 
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address


ARGUMENT_RUN = 
ARGUMENT_RUN_TEST =

################################################################################
########							Sources 							########
################################################################################

SRCS_PATH 	=	srcs/

INCS =	includes			\
		libft/includes		\
		minilibx			\
		
SRCS =	

################################################################################
########							Libraries							########
################################################################################

LIBS =	libft/libft.a			\
		minilibx/libmlx_Linux.a	\

################################################################################
########						Objects/Dependences						########
################################################################################

OBJS_PATH =	objs/

OBJS =	$(addprefix $(OBJS_PATH), $(SRCS:.c=.o))
OBJS_TEST = $(addprefix $(OBJS_PATH), $(SRCS_TEST:.c=.o))
DEPS =	$(addprefix $(OBJS_PATH), $(SRCS:.c=.d))
DEPS_TEST =	$(addprefix $(OBJS_PATH), $(SRCS_TEST:.c=.d))

################################################################################
########							Others								########
################################################################################

RM = rm -rf

################################################################################
########							Colors								########
################################################################################

BLUE		=	\033[0;34m
RED			=	\033[0;31m
GREEN		=	\033[0;32m
NO_COLOR	=	\033[m

################################################################################
########							Rules								########
################################################################################

all: header $(NAME)

bonus: header all

header:
		@echo "${BLUE}"                                                       
		@echo "88b           d88  88               88  88888888ba  888888888888"
		@echo "888b         d888  \"\"               \"\"\" 88      \"8b      88     "
		@echo "88\`8b       d8'88                       88      ,8P      88     "
		@echo "88 \`8b     d8' 88  88  8b,dPPYba,   88  88aaaaaa8P'      88     "
		@echo "88  \`8b   d8'  88  88  88P'   \`\"8a  88  88\"\"\"\"88'        88     "
		@echo "88   \`8b d8'   88  88  88       88  88  88    \`8b        88     "
		@echo "88    \`888'    88  88  88       88  88  88     \`8b       88     "
		@echo "88     \`8'     88  88  88       88  88  88      \`8b      88     "
		@echo "                                                  by The ULTIMATE TEAM"
		@echo "${NO_COLOR}"

$(NAME) : $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(INCS)
	echo "$(BLUE)$(NAME): $(GREEN)Success $(NO_COLOR)"

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -c $< -o $@ $(INCS)

clean :
	$(RM) $(OBJS_PATH)
	make -C minilibx clean
	$(MAKE) -C libft clean

fclean : clean
	$(RM) $(NAME) 
	$(RM) libft/libft.a

re : fclean 
	make -C minilibx re
	make all

################################################################################
#######							Rules for libs							########
################################################################################

libft/libft.a :
	$(MAKE) -C libft all && echo "$(BLUE)Compiation of libft: $(GREEN)Success $(NO_COLOR)" || echo "$(BLUE)Compiation of libft: $(RED)Fail $(NO_COLOR)"

minilibx/libmlx_Linux.a:
	make -C minilibx all && echo "$(BLUE)Compiation of minilibx: $(GREEN)Success $(NO_COLOR)" || echo "$(BLUE)Compiation of libft: $(RED)Fail $(NO_COLOR)"

-include $(DEPS)

.PHONY: all clean fclean re bonus

.SILENT: