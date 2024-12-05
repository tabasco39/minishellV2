/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:47:25 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/27 15:05:04 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_handle_exit_status(t_var *var)
{
	if (WIFSIGNALED(var->current_status))
	{
		if (WTERMSIG(var->current_status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
		else if (WTERMSIG(var->current_status) == SIGINT)
			ft_putstr_fd("\n", STDOUT_FILENO);
		var->status = 128 + WTERMSIG(var->current_status);
	}
	else if (WIFEXITED(var->current_status))
		var->status = WEXITSTATUS(var->current_status);
}

void	ft_display_error(char *error, char *token)
{
	t_var	*all_var;

	all_var = ft_get_struct_var();
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(error, 2);
	ft_putendl_fd(token, 2);
	ft_free_minishell(all_var);
	exit(2);
}

void	ft_display_error_without_exit(char *error, char *token, int status,
		t_var *var)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(error, 2);
	ft_putendl_fd(token, 2);
	var->status = status;
}

t_token	*ft_find_redirection(t_token *token)
{
	t_token	*tmp;

	tmp = ft_find_cmd(token, heredoc);
	if (tmp)
		return (tmp);
	tmp = token;
	while (tmp)
	{
		if (tmp->command >= redirect_input
			&& tmp->command <= append_redirect_output)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_init_exec(t_exec *it, t_var *var)
{
	it->start = 0;
	it->i = 0;
	it->end = ft_count_special_char(var->token, e_pipe);
}
