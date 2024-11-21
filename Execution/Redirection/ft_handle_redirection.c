/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:32:24 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/21 15:53:57 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_handle_output_redirection(t_token *target, int *output_fd,
		char *check_ambigous)
{
	t_var	*var;

	var = ft_get_struct_var();
	if (target->command == redirect_output)
	{
		if (ft_free_in_ambigous(var, check_ambigous) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		*output_fd = open(target->next->token, O_CREAT | O_RDWR | O_TRUNC,
				S_IRWXU);
		ft_check_access_directory(check_ambigous, target);
		dup2(*output_fd, STDOUT_FILENO);
		close(*output_fd);
	}
	if (target->command == append_redirect_output)
	{
		if (ft_free_in_ambigous(var, check_ambigous) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		*output_fd = open(target->next->token, O_CREAT | O_RDWR | O_APPEND,
				S_IRWXU);
		ft_check_access_directory(check_ambigous, target);
		dup2(*output_fd, STDOUT_FILENO);
		close(*output_fd);
	}
	return (EXIT_SUCCESS);
}

int	ft_error_redirect_input(t_token *target, char *check_ambigous,
	int *input_fd, t_var *var)
{
	if (check_ambigous == NULL)
	{
		ft_free_minishell(var);
		return (EXIT_FAILURE);
	}
	if (ft_valid_redirect_input(target) == EXIT_FAILURE)
	{
		ft_close_pipe(var);
		if (*input_fd != -1)
			close(*input_fd);
		ft_free_minishell(var);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_handle_input_redirection(t_token *target, char *check_ambigous,
		int *input_fd, t_var *var)
{
	char	*tmp;

	if (target && target->next)
		tmp = ft_del_quote(target->next->token, "\'\"");
	if (target->command == redirect_input)
	{
		if (ft_error_redirect_input(target, check_ambigous, input_fd, var))
			return (free(tmp), EXIT_FAILURE);
		if (*input_fd != -1)
		{
			dup2(*input_fd, STDIN_FILENO);
			close(*input_fd);
		}
		else
		{
			ft_close_pipe(var);
			ft_free_minishell(var);
			return (free(tmp), EXIT_FAILURE);
		}
	}
	return (free(tmp), EXIT_SUCCESS);
}

void	ft_handle_heredoc_redirection(t_token *target, int *hd)
{
	if (target->command == heredoc)
	{
		dup2(*hd, STDIN_FILENO);
		close(*hd);
	}
}

void	ft_handle_redirection(t_var *var, t_token *target,
		int *hd, int *input_redir)
{
	char	*check_ambigous;
	int		output_fd;
	char	*tmp;

	output_fd = -1;
	ft_handle_redirection_error(&check_ambigous, target, var);
	if (target && target->next)
		tmp = ft_del_quote(target->next->token, "\'\"");
	while (target->is_end != 1)
	{
		if (ft_valid_redir(target) == EXIT_FAILURE)
			ft_display_error("Syntax error near ", tmp);
		if (tmp)
		{
			free(tmp);
			tmp = NULL;
		}
		ft_handle_output_redirection(target, &output_fd, check_ambigous);
		ft_handle_input_redirection(target, check_ambigous, input_redir, var);
		ft_handle_heredoc_redirection(target, hd);
		close(*hd);
		target = target->next;
	}
}
