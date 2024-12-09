/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:47:54 by aelison           #+#    #+#             */
/*   Updated: 2024/12/05 10:48:04 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_list_to_not_expand(char *check)
{
	char	*value;
	char	*result;
	int		flag;
	int		i;

	i = 0;
	flag = -1;
	result = ft_strdup("\0");
	if (!check)
		return (result);
	while (check[i] != '\0')
	{
		if (check[i] == '$')
			flag++;
		if (check[i] == '\'' || check[i] == '\"')
		{
			value = ft_list_aux(check, &i, check[i], &flag);
			result = ft_strjoin_shell(result, value);
			free(value);
		}
		if (check[i] != '\0')
			i++;
	}
	return (result);
}

char	*ft_div_expand(char *to_change)
{
	char	*result;
	char	*value;
	char	tmp[2];
	int		i;

	i = 0;
	tmp[1] = '\0';
	result = ft_strdup("\0");
	if (!to_change)
		return (result);
	while (to_change[i] != '\0')
	{
		tmp[0] = to_change[i];
		if (tmp[0] == '$')
		{
			value = ft_div_expand_aux(to_change, &i, tmp);
			result = ft_strjoin_shell(result, value);
			free(value);
		}
		else
			result = ft_strjoin_shell(result, tmp);
		if (to_change[i] != '\0')
			i++;
	}
	return (result);
}

static void	ft_expand_aux(char **tmp)
{
	int	i;

	i = 0;
	while (tmp && tmp[i])
	{
		free(tmp[i]);
		i++;
	}
	free(tmp);
}

char	*ft_expand_parse(t_var *var, char *to_exp, char *not_exp)
{
	char	*divide;
	char	**tmp;
	char	*result;
	int		i;

	i = 0;
	divide = ft_div_expand(to_exp);
	tmp = ft_split_shell(divide, ' ');
	while (tmp[i])
	{
		if (ft_find_char(not_exp, i + '0') == -1)
			tmp[i] = ft_expand(var, tmp[i], EXIT_SUCCESS);
		i++;
	}
	i = 0;
	result = ft_strdup("\0");
	while (tmp[i])
	{
		result = ft_strjoin_shell(result, tmp[i]);
		i++;
	}
	free(divide);
	ft_expand_aux(tmp);
	return (result);
}

void	ft_join_until_close_quote(char **result,
		char *check, int *i, char quote[2])
{
	char	to_add[2];

	to_add[1] = '\0';
	if (ft_is_open(check, *i + 1) == EXIT_SUCCESS)
	{
		(*i)++;
		*result = ft_strjoin_shell(*result, quote);
		while (check[*i] && check[*i] != quote[0])
		{
			to_add[0] = check[*i];
			*result = ft_strjoin_shell(*result, to_add);
			(*i)++;
		}
	}
}
