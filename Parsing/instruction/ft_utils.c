/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 08:09:13 by aelison           #+#    #+#             */
/*   Updated: 2024/11/25 12:45:45 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_check_double_pipe(char *to_check)
{
	int	i;

	i = 0;
	if (!to_check)
		return (EXIT_SUCCESS);
	while (to_check[i])
	{
		if (to_check[i] == '|')
		{
			i++;
			while (to_check[i])
			{
				while (to_check[i] == ' ')
					i++;
				if (to_check[i] == '|')
					return (EXIT_FAILURE);
				else
					return (EXIT_SUCCESS);
			}
		}
		if (to_check[i])
			i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_is_pipes_closed(char *to_check)
{
	int	len;

	if (!to_check)
		return (EXIT_FAILURE);
	if (to_check && to_check[0] == '\0')
		return (EXIT_FAILURE);
	len = ft_strlen(to_check);
	if (ft_check_double_pipe(to_check) == EXIT_FAILURE)
		return (-1);
	while (--len >= 0)
	{
		if (to_check[len] && to_check[len] == '|')
		{
			len++;
			while (to_check[len])
			{
				if (to_check[len] != ' ')
					return (EXIT_SUCCESS);
				len++;
			}
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
