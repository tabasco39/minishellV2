/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:14:33 by aelison           #+#    #+#             */
/*   Updated: 2024/10/10 09:37:01 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_parse_aux(t_token *current, t_token *nxt)
{
	ft_parse_arg(current, nxt);
	ft_redirection(current, nxt);
	ft_affect_dollar(current);
	if (current->command == option)
	{
		if (current->prev == NULL)
			current->command = not_comm;
		else if (nxt)
		{
			if (!(nxt->command >= e_pipe
					&& nxt->command <= append_redirect_output))
				nxt->command = argument;
		}
	}
	if (current->command == e_pipe && nxt)
	{
		if (current->prev == NULL)
			current->command = not_comm;
		else
		{
			nxt->is_head = 1;
			current->prev->is_end = 1;
		}
	}
	if (nxt == NULL)
		current->is_end = 1;
}

void	ft_parse(t_var *var, t_token *token)
{
	t_token	*current;
	char	**all_path;
	char	*tmp;

	if (!token || !env)
		return ;
	current = token;
	current->is_head = 1;
	while (current)
	{
		if (current->is_head == 1 && current->command == not_comm)
		{
			all_path = ft_get_all_path(var->env, current->token);
			tmp = ft_verify_exec_cmd(all_path);
			if (tmp != NULL)
				current->command = in_sys;
			free(tmp);
		}
		ft_parse_aux(current, current->next);
		ft_parse_sys(current, current->next);
		current = current->next;
	}
}
