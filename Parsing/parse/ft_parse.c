/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:14:33 by aelison           #+#    #+#             */
/*   Updated: 2024/11/19 11:50:48 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_parse_prog(t_token *current)
{
	char	path[1024];
	char	*new;

	if (current->command == in_sys && ft_strncmp(current->token, "./", 2) == 0)
	{
		if (current->token[2] != '\0')
		{
			getcwd(path, 1024);
			new = ft_strjoin(path, current->token + 1);
			free(current->token);
			current->token = new;
			current->command = in_sys;
		}
	}
}

static void	ft_parse_option(t_token *current)
{
	if (current->command == option)
	{
		if (ft_strlen(current->token) == 1)
			current->command = argument;
		else if (ft_isalpha(current->token[1]) == 0)
			current->command = argument;
		else if (current->prev == NULL)
			current->command = argument;
	}
}

static void	ft_prs_aux(t_token *current, t_token *nxt)
{
	if (current->command == dollar)
	{
		if (ft_strlen(current->token) == 1)
			current->command = in_sys;
	}
	if (nxt == NULL)
		current->is_end = 1;
}

static void	ft_parse_aux(t_token *current, t_token *nxt)
{
	ft_parse_prog(current);
	ft_parse_arg(current, nxt);
	ft_redirection(current, nxt);
	ft_affect_dollar(current);
	ft_parse_option(current);
	if (current->command == e_pipe)
	{
		if (current->prev == NULL)
			current->command = not_comm;
		else
		{
			if (nxt)
				nxt->is_head = 1;
			current->prev->is_end = 1;
		}
	}
	ft_prs_aux(current, nxt);
	if (current->is_head == 1 && current->command == dollar)
		current->command = argument;
}

void	ft_parse(t_token *token)
{
	char	*tmp;

	if (!token || !env)
		return ;
	token->is_head = 1;
	tmp = NULL;
	while (token)
	{
		if (token->command == not_comm)
			token->command = in_sys;
		ft_parse_aux(token, token->next);
		ft_parse_sys(token, token->next);
		token = token->next;
	}
}
