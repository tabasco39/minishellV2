/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 06:36:49 by aelison           #+#    #+#             */
/*   Updated: 2024/12/06 14:58:58 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_minishell(t_var *all_var)
{
	if (all_var->iteration->here_doc_fd[0] != -1)
		close (all_var->iteration->here_doc_fd[0]);
	if (all_var->iteration->here_doc_fd[1] != -1)
		close (all_var->iteration->here_doc_fd[1]);
	if (all_var->history != -1)
		close(all_var->history);
	if (all_var->iteration->redir_in_fd != 0
		&& all_var->iteration->redir_in_fd != -1)
		close (all_var->iteration->redir_in_fd);
	ft_lstclear_instru(&all_var->instru, &all_var->token);
	ft_lstclear(&all_var->env, free);
	ft_free_all(all_var->tab_env);
	free(all_var->line);
	free(all_var->iteration);
	if (all_var->hdoc_line)
		free(all_var->hdoc_line);
}

void	ft_lstclear_shell(t_token **head)
{
	t_token	*tmp;

	while (head && *head)
	{
		tmp = *head;
		(*head) = (*head)->next;
		free(tmp->token);
		free(tmp);
	}
	*head = NULL;
}

void	ft_free_all(char **split)
{
	int	i;

	i = 0;
	if (split && *split)
	{
		while (split[i])
		{
			free(split[i]);
			i++;
		}
		free(split);
	}
}

void	ft_close_pipe(t_var *var)
{
	if (var->iteration->here_doc_fd[0] != -1)
		close (var->iteration->here_doc_fd[0]);
	if (var->iteration->here_doc_fd[1] != -1)
		close (var->iteration->here_doc_fd[1]);
	if (var->iteration->pipefd[0] != -1)
		close(var->iteration->pipefd[0]);
	if (var->iteration->pipefd[1] != -1)
		close(var->iteration->pipefd[1]);
}
