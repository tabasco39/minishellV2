/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 08:57:29 by aelison           #+#    #+#             */
/*   Updated: 2024/11/25 10:08:56 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_get_case(char *arg, int i)
{
	if (ft_isalpha(arg[i]) != 0 || arg[i] == '_')
		return (0);
	else if (ft_check_case("@$!?~%/+0123456789", arg + i) == EXIT_SUCCESS)
		return (1);
	return (2);
}

static char	*ft_expand_aux(t_var *var, char *to_change, int *i)
{
	char	*res;
	char	*key;
	int		exec_case;
	int		end;
	int		start;

	res = NULL;
	start = (*i) + 1;
	end = ft_get_dollar_key(to_change, start);
	exec_case = ft_get_case(to_change, start);
	if (exec_case < 2)
		res = ft_exec_case(var, to_change, i, exec_case);
	else
	{
		key = ft_substr(to_change, *i, end - *i);
		res = ft_strdup(key);
		*i += (end - start);
		free(key);
	}
	return (res);
}

static char	*ft_value(t_var *var, char *check, int *ind_dollar)
{
	char	*result;
	int		i;

	result = NULL;
	i = (*ind_dollar) - 1;
	while (i >= 0 && (check[i] == ' ' || check[i] == '\''
			|| check[i] == '\"' || check[i] == '$'))
		i--;
	if (i < 0)
		i = 0;
	if (i >= 0 && check[i] != '<' && check[i] != '>')
		result = ft_expand_aux(var, check, ind_dollar);
	else
		result = ft_strdup("$");
	return (result);
}

static char	*ft_loop(t_var *var, char *to_change, int do_exp)
{
	int		i;
	char	*result;
	char	*tmp;

	i = 0;
	result = ft_strdup("\0");
	while (to_change[i] != '\0')
	{
		if (to_change[i] == '$'
			&& ft_exec_exp(to_change, i, do_exp) == EXIT_SUCCESS)
		{
			tmp = ft_value(var, to_change, &i);
			result = ft_strjoin_shell(result, tmp);
			free(tmp);
		}
		else
		{
			tmp = ft_substr(to_change, i, 1);
			result = ft_strjoin_shell(result, tmp);
			free(tmp);
		}
		i++;
	}
	return (result);
}

char	*ft_expand(t_var *var, char *to_change, int do_exp)
{
	char	*result;

	result = ft_loop(var, to_change, do_exp);
	free(to_change);
	return (result);
}
