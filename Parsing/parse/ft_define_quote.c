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

char	*ft_dollar_define(char *to_change)
{
	char	*result;
	char	tmp[2];
	int		do_join;
	int		i;

	i = 0;
	result = ft_strdup("\0");
	do_join = 0;
	while (to_change[i] != '\0')
	{
		if (to_change[i] == '$')
			do_join = ft_dollar_aux(to_change, i);
		else
			do_join = 0;
		tmp[0] = to_change[i];
		tmp[1] = '\0';
		if (do_join == 0)
			result = ft_strjoin_shell(result, tmp);
		i++;
	}
	return (result);
}

char	*ft_quote_aux(char *to_change)
{
	char	*result;
	char	*to_del;
	char	current[2];
	int		i;

	i = 0;
	to_del = ft_strdup("\0");
	current[0] = '\0';
	current[1] = '\0';
	while (to_change[i] != '\0')
	{
		if (to_change[i] == '\'' || to_change[i] == '\"')
			to_del = ft_dq_redefine(to_change, current, to_del, i);
		i++;
	}
	result = ft_del_quote(to_change, to_del);
	free(to_del);
	return (result);
}

char	*ft_define_quote(char *to_change)
{
	char	*result;
	char	*tmp;

	result = NULL;
	if (!to_change)
		return (NULL);
	if (ft_find_char(to_change, '\'') == -1
		&& ft_find_char(to_change, '\"') == -1)
		return (ft_strdup(to_change));
	tmp = ft_dollar_define(to_change);
	if (tmp[0] == '\0')
		result = ft_quote_aux(to_change);
	else
		result = ft_quote_aux(tmp);
	free(tmp);
	return (result);
}
