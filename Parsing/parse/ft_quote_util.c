/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quote_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:02:12 by aelison           #+#    #+#             */
/*   Updated: 2024/11/20 10:20:08 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_dollar_aux(char *to_check, int i)
{
	int	result;

	result = 0;
	if (!to_check)
		return (0);
	if (to_check[i + 1] == '\'' || to_check[i + 1] == '\"')
	{
		if (ft_is_open(to_check, i + 2)
			== EXIT_SUCCESS)
			result = 1;
	}
	return (result);
}

char	*ft_dq_redefine(char *to_check, char curr[2], char *join, int i)
{
	char	*result;

	result = NULL;
	if (curr[0] != '\0')
	{
		if (ft_is_open(to_check, i + 1) == EXIT_FAILURE)
		{
			result = ft_strjoin_shell(join, curr);
			curr[0] = '\0';
		}
	}
	else
		curr[0] = to_check[i];
	if (result == NULL)
		result = join;
	return (result);
}
