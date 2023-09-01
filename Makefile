CC			=	gcc
CC_FLAGS	=	-Wall -Wextra -Werror -fno-builtin

DIR_HEADERS	=	./includes/
DIR_SRCS	=	./srcs/
DIR_OBJS	=	./compiled_srcs/

SRCS		=	ft_nm.c \
				check.c \
				error.c \
				32bits/handler.c \
				32bits/section.c \
				32bits/symbol.c \
				64bits/handler.c \
				64bits/section.c \
				64bits/symbol.c

INCLUDES	=	ft_nm.h

OBJS		=	$(SRCS:%.c=$(DIR_OBJS)%.o)
DEPS		=	$(SRCS:%.c=$(DIR_OBJS)%.d)

NAME		=	ft_nm

ifeq ($(BUILD),debug)
	CC_FLAGS		+=	-DDEBUG -g3 -fsanitize=address
	DIR_OBJS		=	./debug-compiled_srcs/
	NAME			=	debug-ft_nm
endif

all:		$(NAME)

$(NAME):	$(OBJS) $(addprefix $(DIR_HEADERS), $(INCLUDES))
			$(CC) $(CC_FLAGS) -I $(DIR_HEADERS) $(OBJS) -o $(NAME)

$(OBJS):	| $(DIR_OBJS)

$(DIR_OBJS)%.o:		$(DIR_SRCS)%.c Makefile
					@mkdir -p $(dir $@)
					$(CC) $(CC_FLAGS) -MMD -MP -I $(DIR_HEADERS) -c $< -o $@
-include			$(DEPS)

$(DIR_OBJS):
				mkdir -p $(DIR_OBJS)

clean:
				rm -rf $(DIR_OBJS)

fclean:			clean
				rm -rf $(NAME)

re:				fclean
				$(MAKE) --no-print-directory

.PHONY:			all clean fclean re
