/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:06:14 by lmangall          #+#    #+#             */
/*   Updated: 2023/12/10 14:14:10 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
#include "../../include/expander.h"
#include "../../include/node.h"
#include "../../include/parser.h"
#include "../../lib/libft/src/libft.h"
#include <errno.h>
#include <unistd.h>

void	do_env_builtin(t_data *data)
{
	printf("\n");
	for (int i = 0; i < data->num_vars; i++)
	{
		printf("%-18s", data->vars_container[i].name);
		printf("value: %s\n", data->vars_container[i].value);
	}

	 printf("\n");
	 if (data->num_shell_vars > 0)
		{ 	printf("shell vars (for clarity) :\n");
		 	for (int i = 0; i < data->num_shell_vars; i++)
	 		{
	 			printf("%-18s", data->shell_vars_container[i].name);
	 			printf("value: %s\n", data->shell_vars_container[i].value);
		 	}
		}
}
