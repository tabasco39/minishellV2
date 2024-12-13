/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_dollar_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:44:32 by aelison           #+#    #+#             */
/*   Updated: 2024/11/18 09:52:22 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_expand_res(t_list *env, char *to_change, int start, int len)
{
	char	*tmp;
	char	*result;

	tmp = ft_substr(to_change, start, len);
	result = ft_strdup(ft_expand_to_value(env, tmp));
	free(tmp);
	return (result);
}

char	*ft_expand_to_value(t_list *env, char *to_change)
{
	char	*env_value;

	env_value = ft_getvar(env, to_change);
	if (env_value == NULL)
		return ("\0");
	if (ft_find_char(env_value, '=') == -1)
		return ("\0");
	env_value = env_value + ft_find_char(env_value, '=') + 1;
	return (env_value);
}

int	ft_get_dollar_key(char *to_change, int start)
{
	if (to_change == NULL)
		return (start);
	while (ft_isalnum(to_change[start]) != 0)
		start++;
	if (to_change[start] == '_')
		start++;
	return (start);
}

int	ft_check_case(char *to_process, char *arg)
{
	if (!to_process || !arg)
		return (EXIT_FAILURE);
	if (ft_find_char(to_process, arg[0]) != -1)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

char	*ft_exec_case(t_var *var, char *arg, int *i, int exec_case)
{
	int		start;
	int		end;
	char	*res;
	char	*status;

	res = NULL;
	start = (*i) + 1;
	status = ft_itoa(var->status);
	end = ft_get_dollar_key(arg, start);
	if (exec_case == 0)
	{
		res = ft_expand_res(var->env, arg, start, end - start);
		*i += (end - start);
	}
	else if (exec_case == 1)
	{
		res = ft_exp(var, arg, start, status);
		*i += 1;
	}
	free(status);
	return (res);
}
