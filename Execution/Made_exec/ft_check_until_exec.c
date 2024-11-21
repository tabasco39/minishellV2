/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_until_exec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:51:33 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/21 15:10:19 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_handle_unclosed_pipe(t_var *var, t_instru **tmp, t_exec *iteration)
{
	if (ft_is_pipes_closed(var->line) == EXIT_FAILURE)
	{
		if (ft_find_cmd(var->token, e_pipe) != NULL)
		{
			ft_lstclear_instru(&var->instru, &var->token);
			var->token = NULL;
			var->instru = NULL;
			var->line = ft_pipes_valid(var, var->line);
			if (ft_div_by_token(var) == EXIT_FAILURE)
			{
				free(var->line);
				var->status = 1;
				return (EXIT_FAILURE);
			}
			ft_command_setup(&var->token);
			ft_parse(var->token);
			ft_set_instru(&var->instru, var->token);
			*tmp = var->instru;
			ft_init_exec(iteration, var);
		}
	}
	return (EXIT_SUCCESS);
}

int	ft_check_heredoc(t_token *target, t_exec *iteration, t_var *var)
{
	if (target && target->command == heredoc)
	{
		var->current_status = ft_simul_heredoc(target,
				iteration->here_doc_fd[1], var);
		close(iteration->here_doc_fd[1]);
		if (var->current_status != EXIT_SUCCESS)
		{
			var->status = var->current_status;
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

void	ft_check_redir_input(t_exec *iteration, t_token *curr, t_var *var)
{
	while (curr)
	{
		if (curr && curr->command == redirect_input)
		{
			if (iteration->redir_in_fd != 0 && iteration->redir_in_fd != -1)
				close(iteration->redir_in_fd);
			if (curr->next)
				var->iteration->redir_in_fd = open(curr->next->token, O_RDONLY);
		}
		curr = curr->next;
	}
}

int	ft_check_before_exec(t_instru *tmp, t_token *target, t_exec *iteration,
		t_var *var)
{
	t_token	*curr;

	curr = ft_find_redirection(tmp->start);
	if (ft_check_heredoc(curr, iteration, var) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	ft_check_redir_input(iteration, curr, var);
	curr = target;
	while (curr)
	{
		if (ft_valid_redir(curr) == EXIT_FAILURE)
		{
			ft_display_error_without_exit("Syntax error near ", curr->token,
				2, var);
			return (EXIT_FAILURE);
		}
		curr = curr->next;
	}
	if (ft_is_pipes_closed(var->line) == -1)
	{
		ft_display_error_without_exit("syntax error near ", "|", 2, var);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
