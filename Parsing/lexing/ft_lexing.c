/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:00:50 by aelison           #+#    #+#             */
/*   Updated: 2024/11/14 09:16:38 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_check_cmd_aux(char *token, size_t len)
{
	if (ft_strncmp(token, "cd", ft_max_value(len, ft_strlen("cd"))) == 0)
		return (cd);
	if (ft_strncmp(token, "pwd", ft_max_value(len, ft_strlen("pwd"))) == 0)
		return (pwd);
	if (ft_strncmp(token, "env", ft_max_value(len, ft_strlen("env"))) == 0)
		return (env);
	if (ft_strncmp(token, "echo", ft_max_value(len, ft_strlen("echo"))) == 0)
		return (echo);
	if (ft_strncmp(token, "unset", ft_max_value(len, ft_strlen("unset"))) == 0)
		return (unset);
	if (ft_strncmp(token, "exit", ft_max_value(len, ft_strlen("exit"))) == 0)
		return (e_exit);
	if (ft_strncmp(token, "-", ft_strlen(token)) == 0)
		return (option);
	if (ft_strncmp(token, "|", ft_strlen(token)) == 0)
		return (e_pipe);
	return (not_comm);
}

int	ft_check_cmd(char *token)
{
	int		res;
	size_t	len;

	len = ft_strlen(token);
	if (ft_strncmp(token, "$?", ft_max_value(len, ft_strlen("cd"))) == 0)
		return (question);
	if (ft_strncmp(token, "export", ft_max_value(len,
				ft_strlen("export"))) == 0)
		return (e_export);
	if (ft_strncmp(token, "<<", ft_max_value(len, ft_strlen("cd"))) == 0)
		return (heredoc);
	if (ft_strncmp(token, ">>", ft_max_value(len, ft_strlen("cd"))) == 0)
		return (append_redirect_output);
	if (ft_strncmp(token, ">", ft_max_value(len, ft_strlen("cd"))) == 0)
		return (redirect_output);
	if (ft_strncmp(token, "<", ft_max_value(len, ft_strlen("cd"))) == 0)
		return (redirect_input);
	res = ft_check_cmd_aux(token, len);
	return (res);
}

t_token	*ft_create_token(char *token)
{
	t_token	*result;

	result = malloc(sizeof(t_token));
	if (!result)
		return (NULL);
	result->is_head = 0;
	result->is_end = 0;
	result->command = ft_check_cmd(token);
	result->token = ft_strdup(token);
	result->next = NULL;
	result->prev = NULL;
	return (result);
}

void	ft_add_token(t_token **head, t_token *new_elem, int is_arg)
{
	t_token	*last;

	if (is_arg == EXIT_SUCCESS)
		new_elem->command = argument;
	if (*head == NULL)
		*head = new_elem;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new_elem;
		new_elem->prev = last;
	}
}
