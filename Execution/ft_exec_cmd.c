/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:54:10 by aelison           #+#    #+#             */
/*   Updated: 2024/10/10 08:31:51 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char ft_first_quote(char *word, char first, char second)
{
	int i;

	i = 0;
	if (!word)
		return ('\0');
	while (word[i] != '\0')
	{
		if (word[i] == first || word[i] == second)
			return (word[i]);
		i++;
	}
	return ('\0');
}

void ft_lstclear_instru(t_instru **instru, t_token **head)
{
	t_instru *tmp;

	ft_lstclear_shell(head);
	while (*instru)
	{
		tmp = *instru;
		*instru = (*instru)->next;
		free(tmp);
	}
}

void ft_exit(t_var *var, int end)
{
	rl_clear_history();
	ft_lstclear_instru(&var->instru, &var->token);
	ft_lstclear(&var->env, free);
	ft_free_all(var->tab_env);
	free(var->line);
	ft_putstr_fd("exit : good bye beautiful shell 💩\n", end);
	exit(end);
}

int ft_exec_builtin(t_var *all_var, t_token *start, t_token *end)
{
	int		result;
	int		i;
	char	*tmp;
	t_token	*new_s;

	i = 0;
	tmp = "\n";
	result = EXIT_FAILURE;
	new_s = NULL;
	if (start != end)
		new_s = start->next;
	if (start->command == echo)
	{
		if (new_s && new_s->command == option)
		{
			tmp = new_s->token;
			new_s = new_s->next;
		}
		while (new_s != end && new_s) 
		{
			ft_echo(new_s->token, '\0');
			ft_putstr_fd(" ", 1);
			new_s = new_s->next;
		}
		if (new_s && new_s == end)
			ft_echo(new_s->token, tmp[0]);
		else
			ft_echo("\0", tmp[0]);
		result = EXIT_SUCCESS;
	}
	else if (start->command == cd)
	{
		tmp = ft_getvar(all_var->env, "HOME");
		if (new_s)
		{
			if (new_s->next && (new_s->next->command == argument || new_s->next->command == dollar))
			{
				ft_putstr_fd("minishell: cd: too many arg\n", 2);
				return (EXIT_FAILURE);
			}
			if (new_s->token[0] == '~')
				result = ft_cd(tmp + 5, all_var, "PWD");
			else
				result = ft_cd(new_s->token, all_var, "PWD");
		}
		else
			result = ft_cd(tmp + 5, all_var, "PWD");
	}
	else if (start->command == pwd)
	{
		if (ft_pwd() != NULL)
			result = EXIT_SUCCESS;
		else
			result = EXIT_FAILURE;
	}
	else if (start->command == e_export)
	{
		while (new_s != end && new_s)
		{
			ft_export_aux(all_var, new_s->token);
			new_s = new_s->next;
		}
		if (new_s)
			result = ft_export_aux(all_var, new_s->token);
		else
			result = ft_export_aux(all_var, NULL);
	}
	else if (start->command == unset)
	{
		if (new_s == NULL)
			result = EXIT_SUCCESS;
		else
		{
			while (new_s != end && new_s)
			{
				ft_unset_envp(all_var, new_s->token);
				new_s = new_s->next;
			}
			if (new_s)
				result = ft_unset_envp(all_var, new_s->token);
			else
				ft_putchar_fd('\n', 1);
		}
	}
	else if (start->command == env)
	{
		ft_display_env(&all_var->env, "exist");
		result = EXIT_SUCCESS;
	}
	return (result);
}
