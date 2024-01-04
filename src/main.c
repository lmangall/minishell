/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 20:22:39 by lmangall          #+#    #+#             */
/*   Updated: 2023/12/14 21:57:21 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/builtins.h"
#include "../include/executor.h"
#include "../include/expander.h"
#include "../include/free.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/pipe.h"
#include "../include/redirect.h"
#include "../include/shell.h"
#include "../include/signals.h"
#include "../include/vars.h"
#include "../lib/libft/src/libft.h"
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	int		status;
	t_data	data;
	

	(void)argc;
	(void)argv;
	data.paths = NULL;
	data.envp = NULL;
	init_vars(&data, envp);

	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	status = 1;
	while (status)
	{
		line = readline(SHELL_PROMPT);     // what is returned is malloced and should be freed
		if (line == NULL)
			handle_ctrl_d(SIGQUIT);
		if (line[0] != '\0')
		{
			signal(SIGQUIT, handle_ctrl_backslash);
			add_history(line);
			status = check_and_builtins(line, &data);
			if (status == 1)
				status = parse_and_execute(line, &data);
			if (status == 2)
			{
				free(line);
			    rl_clear_history();
				return (EXIT_SUCCESS);
			}
		}
		signal(SIGQUIT, SIG_IGN);
		// this free might be causing a double free
		//free(line);
	}
    rl_clear_history();
	return (EXIT_SUCCESS);
}

int	parse_and_execute(char *line, t_data *data)
{
	char					**tokens;
	int						status;
	struct node_type_master	*master_node;
	struct node_s			*cmd;

	tokens = lexer(line);
	free(line);
	status = 0;
	if (get_operator(tokens) != NONE)
	{
// have parse_advanced_command return smthing for execution, instead of executing straight away
		master_node = parse_advanced_command(tokens);
		print_master(master_node);
		if (fork() == 0)
			exec_pipe_redir(master_node->root_nodes[0], data);
		waitpid(-1, &status, 0);
		free_ast(master_node);
	}
	else
	{
		signal(SIGINT, handle_ctrl_c_simple_command);
		cmd = parse_simple_command(tokens, data);
		if (fork() == 0)
			exec_pipe_redir(cmd, data);
		waitpid(-1, &status, 0);
		signal(SIGINT, handle_ctrl_c);
		// free_node(cmd);
	}
	free_string_array(tokens);
	return (1);
}