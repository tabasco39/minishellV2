/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_01.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:14:22 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/18 08:31:43 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free_char(char **tmp)
{
	if (*tmp)
		free(*tmp);
	exit(EXIT_FAILURE);
}

int	ft_free_in_ambigous(t_var *var, char *check_ambigous)
{
	if (check_ambigous == NULL)
	{
		ft_free_minishell(var);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
