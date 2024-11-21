/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 13:13:08 by aelison           #+#    #+#             */
/*   Updated: 2024/11/19 11:50:17 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_parse_arg(t_token *current, t_token *nxt)
{
	if (current->command >= env && current->command <= e_exit)
	{
		if (nxt == NULL)
			return ;
		else
		{
			while (nxt && nxt->command == option)
				nxt = nxt->next;
			while (nxt && !(nxt->command >= e_pipe
					&& nxt->command <= append_redirect_output))
			{
				nxt->command = argument;
				nxt = nxt->next;
			}
		}
	}
}

void	ft_affect_dollar(t_token *current)
{
	if (ft_find_char(current->token, '$') != -1)
	{
		if (ft_strlen(current->token) != 1)
			current->command = dollar;
	}
}

void	ft_redirection(t_token *current, t_token *nxt)
{
	if (current->command >= redirect_input
		&& current->command <= append_redirect_output)
	{
		if (nxt)
		{
			if (!(nxt->command >= e_pipe
					&& nxt->command <= append_redirect_output))
				nxt->command = argument;
		}
	}
}

void	ft_parse_sys(t_token *current, t_token *nxt)
{
	if (current->command == in_sys)
	{
		while (nxt && nxt->command == not_comm)
		{
			nxt->command = argument;
			nxt = nxt->next;
		}
	}
}

int	ft_is_char_pair(char *line, char quote)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (line[i] != quote)
		i++;
	while (line[j + i] == quote)
		j++;
	return (j);
}
