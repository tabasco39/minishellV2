/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 06:36:49 by aelison           #+#    #+#             */
/*   Updated: 2024/11/21 10:15:33 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_minishell(t_var *all_var)
{
	close(3);
	close(4);
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
	close (3);
	close (4);
	if (var->iteration->pipefd[0] != -1)
		close(var->iteration->pipefd[0]);
	if (var->iteration->pipefd[1] != -1)
		close(var->iteration->pipefd[1]);
}
