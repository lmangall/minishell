
#include "../../include/vars.h"
#include "../../include/expander.h"
#include "../../include/node.h"
#include "../../include/parser.h"
#include "../../lib/libft/src/libft.h"
#include <errno.h>
#include <unistd.h>

int export_var(t_data *data, const char *name)
{
    int i;
    int j;

    i = 0;
    while (i < data->num_shell_vars)
    {
        if (ft_strcmp(data->shell_vars_container[i].name, name) == 0)
        {
            j = 0;
            while (j < data->num_vars)
            {
                if (ft_strcmp(data->vars_container[j].name, name) == 0)
                    return (0);
                j++;
            }
            if (data->num_vars >= MAX_VARS)
                return (0);
            data->vars_container[data->num_vars] = data->shell_vars_container[i];
            data->num_vars++;
			unset_shell_var(data, name);
            return (1);
        }
        i++;
    }
    return (0);
}

int do_export_builtin(char **tokens, t_data *data)
{
	int	i;

	i = 1;
	while (tokens[i] != NULL)
	{
		export_var(data, tokens[i]);
		i++;
	}
	return (0);
}