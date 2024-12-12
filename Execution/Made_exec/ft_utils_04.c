/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_04.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:28:00 by aranaivo          #+#    #+#             */
/*   Updated: 2024/12/12 08:57:47 by aranaivo         ###   ########.fr       */
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

void	ft_handle_fd(t_exec *iteration, int *input_fd)
{
	if (iteration->i != iteration->start)
		close(*input_fd);
	if (iteration->i != iteration->end)
	{
		if (iteration->pipefd[1] != -1)
			close(iteration->pipefd[1]);
		*input_fd = iteration->pipefd[0];
	}
}

int	ft_handle_access(t_token *target, t_var *var)
{
	if (access(target->next->token, R_OK) == -1)
	{
		if (errno == EACCES)
		{
			ft_putendl_fd("minishell : Permission denied", 2);
			ft_close_pipe(var);
			ft_free_minishell(var);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
