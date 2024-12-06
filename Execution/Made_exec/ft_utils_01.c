/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_01.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:02:23 by aranaivo          #+#    #+#             */
/*   Updated: 2024/12/06 08:48:51 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_dup_fd_not_start_instru(t_exec it, int *input_fd)
{
	if (it.i != it.start)
	{
		if (dup2(*input_fd, STDIN_FILENO) == -1)
		{
			perror("ERROR FOR DUP\n");
			return ;
		}
		close(*input_fd);
	}
}

void	ft_dup_fd_end_instru(t_exec it, int has_redirection)
{
	if (it.i != it.end && has_redirection == 0)
	{
		if (dup2(it.pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("ERROR FOR DUP\n");
			return ;
		}
		if (it.pipefd[1] != -1)
			close(it.pipefd[1]);
		if (it.pipefd[0] != -1)
			close (it.pipefd[0]);
	}
	if (it.pipefd[0] != -1)
		close (it.pipefd[0]);
}

char	**ft_get_execve_parameter(t_instru *current)
{
	t_token	*tmp;
	char	**result;
	int		i;

	i = 0;
	tmp = current->start;
	result = malloc(sizeof(char *) * (ft_count_token_in_instru(current) + 1));
	while (i < ft_count_token_in_instru(current))
	{
		result[i] = ft_strdup(tmp->token);
		if (tmp->command >= redirect_input
			&& tmp->command <= append_redirect_output)
		{
			result[i] = NULL;
			return (result);
		}
		i++;
		tmp = tmp->next;
	}
	result[i] = NULL;
	return (result);
}

t_token	*ft_find_cmd_token(t_instru *instru)
{
	t_token	*result;
	int		i;

	result = instru->start;
	i = ft_count_token_in_instru(instru);
	while (i > 0)
	{
		if (result->command == heredoc)
			return (result);
		if (result->command >= redirect_input
			&& result->command <= append_redirect_output)
			return (result);
		i--;
		result = result->next;
	}
	return (NULL);
}

t_token	*ft_find_cmd(t_token *start, t_comm to_find)
{
	t_token	*tmp;

	tmp = start;
	while (tmp)
	{
		if (tmp->command == to_find)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
