/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:32:24 by aranaivo          #+#    #+#             */
/*   Updated: 2024/12/06 08:40:58 by aranaivo         ###   ########.fr       */
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
		if (ft_redir_error(check_ambigous, target, output_fd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (target->command == append_redirect_output)
	{
		if (ft_free_in_ambigous(var, check_ambigous) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		*output_fd = open(target->next->token, O_CREAT | O_RDWR | O_APPEND,
				S_IRWXU);
		if (ft_redir_error(check_ambigous, target, output_fd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
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
	if (target->command == redirect_input)
	{
		if (access(target->next->token, R_OK) == -1)
		{
			if (errno == EACCES)
			{
				ft_putendl_fd("minishell : Permission denied", 2);
				close(*input_fd);
				ft_close_pipe(var);
				ft_free_minishell(var);
				return (EXIT_FAILURE);
			}
		}
		if (ft_error_redirect_input(target, check_ambigous, input_fd, var))
			return (EXIT_FAILURE);
		if (*input_fd != -1 && dup2(*input_fd, STDIN_FILENO) == -1)
		{
			close(*input_fd);
			ft_close_pipe(var);
			ft_free_minishell(var);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

void	ft_handle_heredoc_redirection(int *hd)
{
	if (dup2(*hd, STDIN_FILENO) == -1)
		return ;
	if (*hd != -1)
		close(*hd);
}

void	ft_handle_redirection(t_var *var, t_token *target,
		int *hd, int *input_redir)
{
	char	*check_ambigous;
	int		output_fd;

	output_fd = -1;
	while (target)
	{
		ft_handle_redirection_error(&check_ambigous, target, var);
		if (ft_valid_redir(target) == EXIT_FAILURE)
			ft_display_error("Syntax error near ", target->token);
		if (ft_handle_output_redirection(target, &output_fd, check_ambigous)
			== EXIT_FAILURE)
			exit(EXIT_FAILURE);
		if (ft_handle_input_redirection(target, check_ambigous,
				input_redir, var) == EXIT_FAILURE)
			exit (EXIT_FAILURE);
		ft_handle_heredoc_redirection(hd);
		if (*hd != -1)
			close(*hd);
		if (target->is_end == 1)
			break ;
		target = target->next;
	}
}
