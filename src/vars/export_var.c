/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:57:22 by lmangall          #+#    #+#             */
/*   Updated: 2024/01/07 19:15:55 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expander.h"
#include "../../include/free.h"
#include "../../include/parser.h"
#include "../../include/parser_nodes.h"
#include "../../include/vars.h"
#include "../../lib/libft/src/libft.h"
#include <errno.h>
#include <unistd.h>

int	export_var(t_data *data, const char *name)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->num_shell_vars)
	{
		if (ft_strcmp(data->shell_vc[i].name, name) == 0)
		{
			j = 0;
			while (j < data->num_vars)
			{
				if (ft_strcmp(data->vc[j].name, name) == 0)
				{
					set_existing_var(data, j, data->shell_vc[i].value);
					unset_shell_var(data, name);
				}
				j++;
			}
			data->vc[data->num_vars] = data->shell_vc[i];
			data->num_vars++;
			unset_shell_var(data, name);
		}
		i++;
	}
	return (0);
}

int	do_export_builtin(char **tokens, t_data *data)
{
	int	i;

	i = 1;
	while (tokens[i] != NULL)
	{
		export_var(data, tokens[i]);
		i++;
	}
	free_string_array(tokens);
	return (-1);
}
