/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:16:57 by lmangall          #+#    #+#             */
/*   Updated: 2024/01/06 15:17:11 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H
# include "../include/executor.h"
# include "../include/lexer.h"
# include "../include/parser.h"
# include "../include/vars.h"
# include "../lib/libft/src/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <string.h>

# define SHELL_PROMPT "\033[1;35mminishell$ \033[0m"

# define EOF (-1)
# define ERRCHAR (0)
# define INIT_SRC_POS (-2)

/**
 * @brief Set signal handlers for the shell.
 */
void	set_signal_handlers(void);

/**
 * @brief Prepare a command for execution,
	including signal handling and variable expansion.
 *
 * @param line A pointer to the command line.
 * @param data A pointer to the data structure containing relevant information.
 */
void	prepare_command_execution(char **line, t_data *data);

/**
 * @brief Handle built-ins and parsing for the given command line.
 *
 * @param line A pointer to the command line.
 * @param data A pointer to the data structure containing relevant information.
 * @return The status of the execution.
 */
int		builtins_to_parsing(char *line, t_data *data);

/**
 * @brief Clean up resources and exit the shell.
 *
 * @param line A pointer to the command line (may be NULL if no command line).
 */
void	cleanup_and_exit(char *line);

/**
 * @brief The main function of the shell.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @param envp An array of environment variable strings.
 * @return The exit status of the shell.
 */
int		main(int argc, char **argv, char **envp);

#endif