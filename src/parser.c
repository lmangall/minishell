/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 18:27:44 by lmangall          #+#    #+#             */
/*   Updated: 2023/12/10 18:29:38 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fcntl.h"
#include "../include/free.h"
#include "../include/lexer.h"
#include "../include/node.h"
#include "../include/parser.h"
#include "../include/expander.h"
#include "../include/shell.h"
#include "../include/vars.h"
#include "../lib/libft/src/libft.h"
#include <unistd.h>

/**
 * @brief Parses a simple command.
 *
 * This function takes an array of tokens and a data structure as input,
 * and it generates a one level linked list representing the parsed command.
 * The `expand` function is called to handle variable expansion. 
 *
 *There is one empty root node of type "ROOT"
 *There is one one node per word of type "WORD"
 *
 * @param tokens An array of tokens representing the command.
 * @param data A data structure containing information about variables.
 * @return A pointer to the root node of the generated one level linked list
 */
struct node_s	*parse_simple_command(char **tokens, t_data *data)
{
	int				i;
	struct node_s	*root;
	struct node_s	*var;

	i = 0;
	root = new_node(ROOT);//THIS IS MALLOCED AND SHOULD BE FREED
	if (!root)
		return (NULL);
	while (tokens[i] != NULL)
	{
		var = new_node(VAR);
		if (!var)
			return (NULL);
		set_node_str(var, tokens[i]);
// printf("var->str bfr expd = %s\n", var->str);
		expand(&var, data);
//printf("var->str aft expd = %s\n", var->str);
// printf("\n");
		add_child_node(root, var);
		i++;
	}
	return (root);
}

int	is_operator(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (1);
	if (ft_strcmp(str, ">") == 0)
		return (1);
	if (ft_strcmp(str, ">>") == 0)
		return (1);
	if (ft_strcmp(str, "<") == 0)
		return (1);
	if (ft_strcmp(str, "<<") == 0)
		return (1);
	return (0);
}

/**
 * @brief Looks for the first operator
 * in the array of tokens and returns the operator type.
 *

	* This function iterates over the array of tokens and determines the operator type
 * based on the token values. The following operators are supported:
 * - PIPE: "|"
 * - RDR_OUT_REPLACE: ">"
 * - RDR_OUT_APPEND: ">>"
 * - RDR_INPUT: "<"
 * - RDR_INPUT_UNTIL: "<<"
 *
 * @param token An array of tokens to search for operators.
 * @return The operator type, or NONE if no operator is found.
 */
t_operator	get_operator(char **token)
{
	int	i;

	i = 0;
	while (token[i] != NULL)
	{
		if (ft_strnstr(token[i], ">>", 2))
			return (RDR_OUT_APPEND);
		if (ft_strnstr(token[i], "<<", 2))
			return (RDR_INPUT_UNTIL);
		if (ft_strnstr(token[i], "|", 1))
			return (PIPE);
		if (ft_strnstr(token[i], ">", 1))
			return (RDR_OUT_REPLACE);
		if (ft_strnstr(token[i], "<", 1))
			return (RDR_INPUT);
		i++;
	}
	return (NONE);
}

struct node_type_master	*parse_advanced_command(char **tokens)
{
	int						i;
	int						rdr_input;
	int						rdr_output;
	struct node_s			*head;
	struct node_s			*current_cmd;
	struct node_s			*new_cmd;
	struct node_s			*word;
	struct node_type_master	*master_node;

	i = 0;
	rdr_input = 0;
	rdr_output = 0;
	head = NULL;
	current_cmd = NULL;
	while (tokens[i] != NULL)
	{
		if ((i == 0) || ((is_operator(tokens[i - 1])) && (i > 0)))
		{
			new_cmd = create_root_node(tokens[i]);
			if (!new_cmd)
				return (NULL);
			if (!add_command_node_to_list(&head, &current_cmd, new_cmd))
				return NULL;
			new_cmd->operator= get_operator(tokens + i);
			if (rdr_output == 1 && rdr_input == 1)
			{
				new_cmd->prev_sibling = head;
				new_cmd->operator= RDR_INPUT; // this is getting overwritten
			}
			if (get_operator(tokens + i) == RDR_OUT_REPLACE)
				rdr_output++;
			if (get_operator(tokens + i) == RDR_INPUT)
				rdr_input++;
		}
		else if (is_operator(tokens[i])) //(strcmp(tokens[i], "|") == 0)
		{
			i++;
			i--;
		}
		else  
		{
			// Handle regular words (non-options)
			word = new_node(VAR);
			if (!word)
				return NULL;
			set_node_str(word, tokens[i]);
			if (!add_child_node(current_cmd, word))
				return NULL;
		}
		i++;
	}
	master_node = create_master_node(head);
	if (master_node == NULL)
		return NULL;
	link_root_nodes(master_node);
	return master_node;
}

struct node_s	*create_root_node(char *token)
{
	struct node_s	*new_cmd;
	struct node_s	*cmd_var;

	new_cmd = new_node(ROOT);
	if (!new_cmd)
		return NULL;
	cmd_var = new_node(VAR);
	if (!cmd_var)
		return NULL;
	set_node_str(cmd_var, token);
	if (!add_child_node(new_cmd, cmd_var))
		return NULL;
	return new_cmd;
}

/**
 * @brief Adds a new command node to the list of command nodes.
 *
 * @param cmd Pointer to the head of the list of command nodes.
 * @param current_cmd Pointer to the current command node in the list.
 * @param new_cmd Pointer to the new command node to add to the list.
 *
 * @return 1 if the command node was successfully added to the list,
	0 otherwise.
 *
* This function adds a new command node to the list of command nodes. If the list is empty,
 * the new command node becomes the head of the list. If the list is not empty,
	the new command  node is added to the end of the list.
 */
int	add_command_node_to_list(struct node_s **cmd, struct node_s **current_cmd,
		struct node_s *new_cmd)
{
	if (*cmd == NULL)
	{
		*cmd = new_cmd;
		*current_cmd = *cmd;
	}
	else
	{
		(*current_cmd)->next_sibling = new_cmd;
		*current_cmd = (*current_cmd)->next_sibling;
	}
	return 1;
}

struct node_type_master	*create_master_node(struct node_s *cmd)
{
	struct node_type_master	*master_node;
	struct node_s			*current_cmd;

	master_node = malloc(sizeof(struct node_type_master));
	if (!master_node)
		return NULL;
	master_node->type = MASTER;
	master_node->str = NULL;
	master_node->nbr_root_nodes = 0;
	master_node->root_nodes = NULL;
	current_cmd = cmd;
	while (current_cmd != NULL)
	{
		master_node->nbr_root_nodes++;
		master_node->root_nodes = realloc(master_node->root_nodes,
				sizeof(struct node_s *) * master_node->nbr_root_nodes);
		master_node->root_nodes[master_node->nbr_root_nodes - 1] = current_cmd;
		current_cmd = current_cmd->next_sibling;
	}
	return master_node;
}

void	link_root_nodes(struct node_type_master *master_node)
{
	int i = 0;
	while (i < master_node->nbr_root_nodes)
	{
		if ((master_node->root_nodes[i]->next_sibling == NULL)
			&& (master_node->root_nodes[i + 1]))
			add_sibling_node(master_node->root_nodes[i],
				master_node->root_nodes[i + 1]);
		i++;
	}
	master_node->root_nodes[master_node->nbr_root_nodes
		- 1]->next_sibling = NULL;
}