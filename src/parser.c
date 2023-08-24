/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 18:27:44 by lmangall          #+#    #+#             */
/*   Updated: 2023/08/24 10:57:20 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../lib/libft/src/libft.h"
#include <unistd.h>
#include "../include/shell.h"
#include "../include/lexer.h"
#include "../include/node.h"
#include "../include/vars.h"
#include "../include/shell.h"
#include "../include/parser.h"


void	init_data(t_data *data)
{
	// data->args = NULL;
	// data->paths = NULL;
	// data->envp = NULL;
	// data->pwd = NULL;

	data->paths = getenv("PATH");
	data->envp = getenv("ENV");


	//CODE PROPOSED BY COPILOT X
	// if there is no path, set a default one:
	// char *default_paths = "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin";
	// data->paths = getenv("PATH");
	// if(!data->paths)
	// 	data->paths = default_paths;

}

int	check_dollar(char *str)
{
	if (str[0] == '$')
		return(1);
	return (0);
}

static int replace_var(struct node_s *node)
{
// 	char 
// 	node->str = get_vars_value(node->str);

// }	
	//ft_memchr
	ft_memcpy(node->str, get_vars_value(node->str), ft_strlen(get_vars_value(node->str)));
	return (0);
}

//a function that transverses  the AST and use the check_dollar function to identify a dollar and print a statement
//the function is called expansion 
int expansion(struct node_s *node)
{
	if (node->type == NODE_VAR)
	{
		if (check_dollar(node->str) == 1)
		{
			printf("Dollar found in %s\n", node->str);
			//print_vars(); //    THIS SEGAULTS
			replace_var(node);
			printf("str after replace_var: %s\n", node->str);
		}
	}
	if (node->first_child)
		expansion(node->first_child);
	if (node->next_sibling)
		expansion(node->next_sibling);
	return (0);
}


struct node_s *parse_simple_command(char **tokens)
{
	int i = 0;
	
	struct node_s *cmd = new_node(NODE_COMMAND);
	if(!cmd)
		return NULL;
	while(tokens[i] != NULL)
	{
		struct node_s *word = new_node(NODE_VAR);
		if (!word)
			return NULL;
		set_node_str(word, tokens[i]);
		add_child_node(cmd, word);
		i++;
	}
		return cmd;
}