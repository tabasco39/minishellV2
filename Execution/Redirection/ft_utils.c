/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:29:30 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/27 15:10:17 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_check_access_directory(char *check_ambigous, t_token *target)
{
	int		fd;
	t_var	*all_var;

	all_var = ft_get_struct_var();
	fd = open(target->next->token, O_RDONLY);
	if (check_ambigous && (fd == -1 && access(target->next->token, W_OK) == -1
			&& target->command != heredoc))
	{
		ft_putstr_fd("minishell : ", 2);
		ft_putstr_fd(target->next->token, 2);
		ft_putendl_fd(": Permission denied ", 2);
		if (fd != -1)
			close(fd);
		ft_free_minishell(all_var);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	ft_handle_redirection_error(char **check_ambigous, t_token *target,
		t_var *var)
{
	if (target->command >= redirect_input
		&& target->command <= append_redirect_output)
	{
		if (target && target->next)
		{
			*check_ambigous = ft_check_ambigous(var, target->next);
			if (*check_ambigous)
			{
				free(target->next->token);
				target->next->token = *check_ambigous;
			}
		}
		if (target && target->next == NULL)
		{
			ft_putstr_fd("minishell : error : Syntax error near ", 2);
			ft_putendl_fd(target->token, 2);
			ft_free_minishell(var);
			exit(2);
		}
	}
}

char	*ft_check_ambigous(t_var *var, t_token *token)
{
	char	*tmp;
	char	*del_quote;
	int		is_ambigue;

	is_ambigue = ft_is_ambigous(token->command,
			token->token, ft_find_char(token->token, '$'));
	tmp = ft_expand(var, ft_strdup(token->token), EXIT_FAILURE);
	del_quote = ft_del_quote(tmp, "\"\'");
	free(tmp);
	if (token && token->prev && token->prev->command != heredoc)
	{
		if (is_ambigue == 1)
		{
			if (del_quote[0] == '\0' || ft_find_char(del_quote, ' ') != -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putendl_fd("ambigous redirect", 2);
				free(del_quote);
				return (NULL);
			}
			else
				return (del_quote);
		}
	}
	return (del_quote);
}

int	ft_valid_redir(t_token *target)
{
	if (target->command >= e_pipe && target->command <= append_redirect_output)
	{
		target = target->next;
		if (!target)
			return (EXIT_FAILURE);
		else if (target->command >= e_pipe
			&& target->command <= append_redirect_output)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_valid_redirect_input(t_token *token)
{
	t_token	*current;
	int		fd;

	current = token;
	if (current->command == redirect_input)
	{
		current = current->next;
		if (!current || (current->command >= e_pipe
				&& current->command <= append_redirect_output))
		{
			ft_putendl_fd("minishell: syntax error near unexpected token", 2);
			return (EXIT_FAILURE);
		}
		fd = open(current->token, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(current->token, 2);
			ft_putendl_fd(": No such file or directory", 2);
			return (EXIT_FAILURE);
		}
		close(fd);
	}
	return (EXIT_SUCCESS);
}
