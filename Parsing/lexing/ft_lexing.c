/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:00:50 by aelison           #+#    #+#             */
/*   Updated: 2024/10/08 13:59:40 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_check_cmd_aux(char *token, size_t len)
{
	if (ft_strncmp(token, "cd", len) == 0)
		return (cd);
	if (ft_strncmp(token, "pwd", len) == 0)
		return (pwd);
	if (ft_strncmp(token, "env", len) == 0)
		return (env);
	if (ft_strncmp(token, "echo", len) == 0)
		return (echo);
	if (ft_strncmp(token, "unset", len) == 0)
		return (unset);
	if (ft_strncmp(token, "exit", len) == 0)
		return (e_exit);
	if (ft_strncmp(token, "-", 1) == 0)
		return (option);
	if (ft_strncmp(token, "|", 1) == 0)
		return (e_pipe);
	return (not_comm);
}

int	ft_check_cmd(char *token)
{
	int	res;
	size_t	len;

	len = ft_strlen(token);
	if (ft_strncmp(token, "$?", 2) == 0)
		return (question);
	if (ft_strncmp(token, "export", len) == 0)
		return (e_export);
	if (ft_strncmp(token, "<<", 2) == 0)
		return (delimiter_redirect_input);
	if (ft_strncmp(token, ">>", 2) == 0)
		return (append_redirect_output);
	if (ft_strncmp(token, ">", 1) == 0)
		return (redirect_output);
	if (ft_strncmp(token, "<", 1) == 0)
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

void	ft_add_token(t_token **head, t_token *new_elem)
{
	t_token	*last;

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
