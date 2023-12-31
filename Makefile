# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/09 18:29:39 by lmangall          #+#    #+#              #
#    Updated: 2024/01/07 15:43:04 by lmangall         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
SRCDIR = src
INCDIR = include
LIBFTDIR = lib/libft
OBJDIR = obj

SRCS =	lexer_expander/lexer.c \
		lexer_expander/lexer_quotes.c \
		lexer_expander/expander.c \
		lexer_expander/expander_quotes.c \
		builtins/builtins.c \
		builtins/cd.c \
		builtins/echo.c  \
		builtins/pwd.c \
		builtins/env.c \
		builtins/exit.c \
		parser/parser.c \
		parser/parser_advcd_cmd.c \
		parser/parser_operators.c \
		parser/parser_nodes.c \
		parser/parser_nodes_utils.c \
		executor/executor.c \
		executor/executor_simple.c \
		vars/export_var.c \
		vars/unset_var.c \
		vars/set_var.c \
		vars/set_shell_var.c \
		vars/utils_var.c \
		vars/utils_var_dev.c \
		vars/vars.c \
		main.c \
		signals.c \
		free.c \
		pipe.c \
		redirect.c \
		DEV.c

OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

CC = cc
CFLAGS = -I$(INCDIR) -Wall -Wextra -Werror -g #-fsanitize=address
LIBS = -L$(LIBFTDIR) -lft
READLINE_LIBS = -Lreadline -lreadline -Lhistory -lhistory
READLINE_INC = -I /Users/$(USER)/.brew/opt/readline/include
NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) -C $(LIBFTDIR)
	@$(CC) $(OBJS) $(CFLAGS) $(LIBS) $(READLINE_LIBS) -o $(NAME)
	@echo "\033[32mCompilation succeeded!\033[0m"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@) # Create the subdirectories
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFTDIR) clean
	@$(RM) -r $(OBJDIR) # Remove the object directory

fclean: clean
	@$(MAKE) -C $(LIBFTDIR) fclean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

install_readline:
	apt-get install libreadline-dev
	@$(MAKE) all READLINE_LIBS="$(READLINE_LIBS)" READLINE_INC="$(READLINE_INC)"

readline: install_readline

.PHONY: install_readline readline