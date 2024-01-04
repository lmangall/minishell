/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 18:11:09 by lmangall          #+#    #+#             */
/*   Updated: 2024/01/04 12:48:32 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/shell.h"
#include "../../include/fcntl.h"
#include "../../include/vars.h"
#include "../../include/expander.h"
#include "../../include/node.h"
#include "../../include/parser.h"
#include "../../lib/libft/src/libft.h"
#include <errno.h>
#include <unistd.h>

// CHECK IF THE ERASE QUOTES IS WORKING PROPERLY
char	**lexer(char *line)
{
	char	**tokens;
	if (!line)
		return (NULL);
	if (contains_two(line, '\"') || contains_two(line, '\''))
		check_quotes_replace_ff(line);
	tokens = ft_split(line, ' ');
	put_space_back(tokens);
	char	**new_tokens = erase_quotes(tokens);
	// print_double_pointer_to_char(tokens);
	return (new_tokens);
    //return(tokens);
}