/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_define_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 06:39:29 by aelison           #+#    #+#             */
/*   Updated: 2024/11/19 09:01:24 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_is_open(char *to_check, int end)
{
	int		i;
	char	is_open;

	i = 0;
	is_open = '\0';
	while (to_check[i] && i != end)
	{
		if (to_check[i] == '\"' || to_check[i] == '\'')
		{
			if (is_open == '\0')
				is_open = to_check[i];
			else
			{
				if (is_open == to_check[i])
					is_open = '\0';
			}
		}
		i++;
	}
	if (is_open == '\0')
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void	ft_quote_case(char *check, char curr[2], char **result, int *i)
{
	char	tmp[2];

	tmp[1] = '\0';
	if (ft_is_open(check, (*i) + 1) == EXIT_SUCCESS)
	{
		(*i)++;
		while (check[*i] && check[*i] != curr[0])
		{
			tmp[0] = check[*i];
			*result = ft_strjoin_shell(*result, tmp);
			(*i)++;
		}
		if (check[*i] == curr[0])
			(*i)++;
	}
	else
	{
		if (check[*i] != '\0')
			(*i)++;
	}
}

static void	ft_define_aux(char **result, char *check, int *i, char curr[2])
{
	if (curr[0] == '\'' || curr[0] == '\"')
		ft_quote_case(check, curr, result, i);
	else
	{
		*result = ft_strjoin_shell(*result, curr);
		if (check[*i] != '\0')
			(*i)++;
	}
}

char	*ft_define_quote(char *to_change)
{
	int		i;
	char	curr[2];
	char	*result;

	i = 0;
	if (!to_change)
		return (NULL);
	if (ft_find_char(to_change, '\'') == -1
		&& ft_find_char(to_change, '\"') == -1)
		return (ft_strdup(to_change));
	result = ft_strdup("\0");
	curr[1] = '\0';
	while (to_change[i] != '\0')
	{
		curr[0] = to_change[i];
		ft_define_aux(&result, to_change, &i, curr);
	}
	return (result);
}
