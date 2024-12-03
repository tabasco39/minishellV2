/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_simul_hdoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:21:06 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/29 14:06:34 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_reinit_next_heredoc(t_token *target, char **here_doc_result)
{
	if (ft_find_cmd(target->next, heredoc) != NULL)
	{
		free(*here_doc_result);
		*here_doc_result = NULL;
		*here_doc_result = ft_strdup_shell("\0");
	}
}

int	ft_join_result(char **buffer, t_token *target, char **here_doc_result)
{
	if (!(*buffer))
	{
		ft_putstr_fd("minishell: warning here-doc: ", 2);
		ft_putendl_fd(target->next->token, 2);
		return (EXIT_FAILURE);
	}
	if (ft_strncmp(target->next->token, *buffer,
			ft_max_value(ft_strlen(target->next->token),
				ft_strlen(*buffer))) != 0)
	{
		*here_doc_result = ft_strjoin_shell(*here_doc_result, *buffer);
		*here_doc_result = ft_strjoin_shell(*here_doc_result, "\n");
		free(*buffer);
	}
	else
	{
		free(*buffer);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	ft_handle_prompt_heredoc(t_var *var, t_token *target, int *bool)
{
	char	*buffer;
	char	*tmp;

	tmp = NULL;
	var->hdoc_line = ft_strdup("\0");
	while (target)
	{
		if (target->command == heredoc)
		{
			tmp = ft_expand_hdoc(target->next->token, bool);
			target->next->token = ft_strdup(tmp);
			free(tmp);
			while (1)
			{
				buffer = readline(RED"HDOC$> "RESET);
				if (ft_join_result(&buffer, target, &var->hdoc_line)
					== EXIT_FAILURE)
					break ;
			}
			ft_reinit_next_heredoc(target, &var->hdoc_line);
		}
		target = target->next;
	}
}

void	ft_get_heredoc_result(t_token *target, int hd, t_var *var)
{
	int		bool;

	bool = 0;
	ft_handle_prompt_heredoc(var, target, &bool);
	if (bool == 0)
	{
		var->hdoc_line = ft_expand(var, var->hdoc_line, EXIT_SUCCESS);
	}
	if (var->hdoc_line[0] != '\0')
	{
		if (ft_find_char(var->hdoc_line, '\n') == -1)
			ft_putendl_fd(var->hdoc_line, hd);
		else
			ft_putstr_fd(var->hdoc_line, hd);
	}
	if (hd != -1)
		close(hd);
	ft_free_minishell(var);
	exit(EXIT_SUCCESS);
}

int	ft_simul_heredoc(t_token *target, int hd, t_var *var)
{
	pid_t	pid;

	if (!target->next || (target->next && target->next->command >= e_pipe
			&& target->next->command <= append_redirect_output))
	{
		return (EXIT_SUCCESS);
	}
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, ft_interupt_and_exit);
		ft_get_heredoc_result(target, hd, var);
	}
	waitpid(pid, &var->current_status, 0);
	ft_handle_exit_status(var);
	return (var->status);
}
