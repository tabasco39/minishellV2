/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_04.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:28:00 by aranaivo          #+#    #+#             */
/*   Updated: 2024/12/06 15:29:55 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_find_cmd_intru(t_instru *tmp, t_comm target)
{
	int		i;
	t_token	*result;

	i = ft_count_token_in_instru(tmp);
	result = tmp->start;
	while (i > 0)
	{
		if (result->command == target)
			return (EXIT_SUCCESS);
		i--;
		result = result->next;
	}
	return (EXIT_FAILURE);
}

void	ft_reinit_heredoc_fd(t_instru *tmp, t_token *target, t_var *var)
{
	if (!target || ft_find_cmd_intru(tmp, heredoc) == EXIT_FAILURE)
	{
		if (!tmp->next)
		{
			var->iteration->here_doc_fd[0] = -1;
			var->iteration->here_doc_fd[1] = -1;
		}
	}
}
