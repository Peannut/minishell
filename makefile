# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zoukaddo <zoukaddo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 17:04:34 by aamoussa          #+#    #+#              #
#    Updated: 2022/09/20 01:02:15 by zoukaddo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= minishell

SRCS			= shell.c token.c parser.c build_nodes.c argument_cleaner.c \
				  $(BUILTIN)echo_cmd.c  $(BUILTIN)env.c $(BUILTIN)cd_cmd.c ./excution/utils/env_handler.c \
				  $(BUILTIN)pwd.c $(BUILTIN)export_cmd.c $(BUILTIN)unset_cmd.c $(BUILTIN)exit_cmd.c ./excution/executer.c  

OBJS			= $(SRCS:.c=.o)

CC				= gcc -g

RM				= rm -f

READLINE_PATH =  -lreadline -L ~/homebrew/opt/readline/lib -I ~/homebrew/opt/readline/include 

BUILTIN 		= ./excution/builtin/

CFLAGS			=  #-fsanitize=address -g #-Wall -Wextra -Werror

all:			$(NAME)

$(NAME):		$(OBJS)
				
				gcc ${CFLAGS} ${OBJS} ./libft/libft.a -o ${NAME} $(READLINE_PATH)

clean:
				$(RM) $(OBJS) 

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re