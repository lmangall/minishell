/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 17:14:51 by lmangall          #+#    #+#             */
/*   Updated: 2023/10/15 13:32:44 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/expander.h"
#include "../include/node.h"
#include "../include/parser.h"
#include "../include/shell.h"
#include "../lib/libft/src/libft.h"
#include <errno.h>
#include <expander.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static void	free_ptr(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

// char	*identify_var(char *str, t_data *data)
// {
// 	int		start;
// 	int		len;
// 	int		i;

// 	i = 0;
// 	start = 0;
// 	if (str == NULL)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		if (str[i] == '$')
// 		{
// 			start = i;
// 			while(str[i] != ' ' && str[i] != '\0' && (str[i] >= 'A' && str[i] <= 'Z'))
// 				i++;
// 			len = i - start;
// 			printf("len = %d\n", len);
// 		}
// 		i++;
// 	}
// 	i = 0;               //is it necessary
// 	while (i < data->num_vars)
// 	{
// 		if (ft_strcmp(str + start + 1, data->vars_container[i].name) == 0)
// 			return (data->vars_container[i].name);
// 		else
// 			i++;
// 	}
// 	return (NULL);
// }


char	*identify_var(char *str, t_data *data)
{
printf("inside identify_var\n");
	int		start;
	int		len;
	int		i;

	i = 0;
	start = 0;
	if (str == NULL)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			start = i;
			while(str[i] != ' ' && str[i] != '\0' && (str[i] >= 'A' && str[i] <= 'Z'))
				i++;
			len = i - start;
			(void)len;
		}
		i++;
	}
	i = 0;               //is it necessary
	while (i < data->num_vars)
	{
// printf(" = str + start + 1 = %s\n", str + start + 1);
		if (ft_strcmp(str + start + 1, data->vars_container[i].name) == 0)
		{
			return (data->vars_container[i].name);
// printf("data->vars_container[i].name = %s\n", data->vars_container[i].name);
		}
		else
			i++;
	}
	return (NULL);
}



char	*expand_var(char *str, t_vars *var, t_data *data)
{
printf("inside expand_var\n");
	char	*expanded_str;
	char	*tmp;
	char	*name;
	char	*value;
	int		i;
	int		j;
	int size;

	i = 0;
	j = 0;
	size = ft_strlen(str) - ft_strlen(var->name) + ft_strlen(var->value) + 1;
	expanded_str = malloc(sizeof(char) * size);
	if (!expanded_str)
		return (NULL);
	name = identify_var(str + i, data);
	if (name)
printf("name = %s\n", name);
	{
		value = find_val(name, data);
		while (str[i] != '\0')
		{
			if (str[i] == '$')
			{
				tmp = ft_strjoin(expanded_str, value);
printf("tmp (expanded+value)= %s\n", tmp);
				// free_ptr(expanded_str);
				expanded_str = tmp;
printf("expanded_str = %s\n", expanded_str);
				i += ft_strlen(expanded_str);
				//i++;
			}
			else
			{
				expanded_str[i] = str[j];
				j++;
				i++;
			}
		}
	}
printf("expanded_str end of func= %s\n", expanded_str);
	// expanded_str[j + 1] = '\0';
printf("expanded_str end of func 2= %s\n", expanded_str);
	return (expanded_str);
}


char	*find_val(char *name, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_vars)
	{
		if (ft_strcmp(name, data->vars_container[i].name) == 0)
			return (data->vars_container[i].value);
		else
			i++;
	}
	return (NULL);
}

void	expand(struct node_s *node, t_data *data)
{
printf("inside expand\n");
	char	*var_name;
	char	*expanded_str;
	char	*tmp;
	t_vars	*var;

	var_name = identify_var(node->str, data);
printf("var_name = %s\n", var_name);
	if (var_name)
	{
printf("var_name in if = %s\n", var_name);
		var = find_var(var_name, data);
// printf("expd var->name = %s\n", var->name);
		expanded_str = expand_var(node->str, var, data);
printf("expanded_str in expd = %s\n", expanded_str);
		tmp = ft_strdup(expanded_str);
printf("tmp in expd = %s\n", tmp);
		free_ptr(node->str);
		node->str = tmp;
printf("node->str in expd = %s\n", node->str);
		free_ptr(expanded_str);
	}
}

t_vars	*find_var(char *name, t_data *data)
{
	int	i;
	t_vars	*control;

	i = 0;
	control = malloc(sizeof(t_vars));
	if (!control)
		return (control);
	while (i < data->num_vars)
	{
		if (ft_strcmp(name, data->vars_container[i].name) == 0)
			return (&data->vars_container[i]);
		else
			i++;
	}
	return (control);
}

// char	*find_val(struct node_s *node, char *str, t_data *data)
// {
// 	char	*value;
// 	char	*var;

// 	var = identify_var(str);
// 	if (var && var_exists(data, var) == 0)
// 	{
// 		if (node != NULL)
// 			node->var_exists = true;
// 		value = search_env_var(data, var);
// 	}
// 	else if (var && var[0] == '?' && var[1] == '=')
// 		value = ft_itoa(g_last_exit_code);
// 	else
// 		value = NULL;
// 	free_ptr(var);
// 	return (value);
// }



// //takes a str that may contain on or several variables
// //returns a malloced string with the variables expanded
// //can use expand_var to expand each variable
// char	*expand_str(char *str, t_data *data)
// {
// 	char	*expanded_str;
// 	char	*tmp;
// 	char	*var;
// 	char	*value;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	expanded_str = malloc(sizeof(char) * ft_strlen(str) + 1);
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '$')
// 		{
// 			var = identify_var(str + i);
// 			value = find_val(NULL, var, data);
// 			if (value)
// 			{
// 				tmp = ft_strjoin(expanded_str, value);
// 				free_ptr(expanded_str);
// 				expanded_str = tmp;
// 				i += ft_strlen(var);
// 			}
// 			else
// 			{
// 				expanded_str[j] = str[i];
// 				j++;
// 				i++;
// 			}
// 			free_ptr(var);
// 		}
// 		else
// 		{
// 			expanded_str[j] = str[i];
// 			j++;
// 			i++;
// 		}
// 	}
// 	expanded_str[j] = '\0';
// 	return (expanded_str);
// }



// char *expand_var(char *node, t_data *data){
//     // Iterate through the variables in data->vars_container
//     for (int i = 0; i < data->num_vars; i++) 
// 	{
//         // Check if the token matches the variable name
//         if (strcmp(node, b. == 0 ||
//             (strlen(node) > 1 && strcmp(node + 1, data->vars_container[i].name) == 0)) {
//             // Return the corresponding variable value
//             return data->vars_container[i].value;
//         }
//     }

//     // Return NULL if the variable is not found
//     return NULL;
// }


// char	*identify_var(char *str)
// {
// 	char	*var;
// 	char	*tmp;
// 	int		start;
// 	int		len;
// 	int		i;

// 	i = 0;
// 	start = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '$')
// 		{
// 			start = i + 1;
// 			break ;
// 		}
// 		i++;
// 	}
// 	len = var_length(str);
// 	var = ft_substr(str, start, len);
// 	if (!var)
// 		return (NULL);
// 	tmp = ft_strjoin(var, "=");
// 	free_ptr(var);
// 	var = tmp;
// 	return (var);
// }















static int	contains_dollar(struct node_s *node)
{
	if (node->str[0] == '$')
		return (1);
	return (0);
}

int	expansion_substitution(struct node_s *node, t_data *data)
{
	int	i;

	i = 0;
	if (contains_dollar(node))
	{
		while (i <= data->num_vars)
		{
			if (ft_strcmp(node->str + 1, data->vars_container[i].name) == 0)
			{
				set_node_str(node, data->vars_container[i].value);
				return (1);
			}
			i++;
		}
	}
	return (0);
}

char	*get_var_name(char *str)
{
	int		i;
	char	*var_name;

	i = 0;
	var_name = malloc(sizeof(char) * strlen(str) + 1);
	while (str[i] != '=')
	{
		var_name[i] = str[i];
		i++;
	}
	var_name[i] = '\0';
	return (var_name);
}

static int	contains_equal(struct node_s *node)
{
	int	i;

	i = 0;
	while (node->str[i] != '\0')
	{
		if (node->str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	expansion_set_var(struct node_s *node, t_data *data)

{
	if (contains_equal(node))
	{
		char *var_name = get_var_name(node->str);
		set_var(data, var_name, node->str + strlen(var_name) + 1);
		return (1);
	}
	return (0);
}