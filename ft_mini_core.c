/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mini_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 10:55:02 by aelison           #+#    #+#             */
/*   Updated: 2024/11/20 14:35:26 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_display_aux(char *to_print)
{
	int	i;
	int	count_char;

	i = 0;
	count_char = 0;
	while (to_print[i] != '\0')
	{
		if (count_char == 1)
		{
			ft_putchar_fd((char)34, STDOUT_FILENO);
			count_char++;
		}
		if (to_print[i] == '=')
			count_char++;
		ft_putchar_fd(to_print[i], STDOUT_FILENO);
		i++;
	}
	if (count_char == 1)
		ft_putstr_fd("\"\"", STDOUT_FILENO);
	else if (count_char > 1)
		ft_putchar_fd('\"', STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	ft_display_export(t_list *env)
{
	char	*tmp;

	while (env)
	{
		tmp = env->content;
		if (ft_strncmp(tmp, "_=", 2) != 0)
		{
			ft_putstr_fd("export ", STDOUT_FILENO);
			ft_display_aux(tmp);
		}
		env = env->next;
	}
}

void	ft_display_env(t_list **env, char *before)
{
	t_list	*tmp;
	char	*content;

	if (before == NULL)
		ft_display_export(*env);
	else
	{
		tmp = *env;
		while (tmp)
		{
			content = tmp->content;
			if (ft_strncmp(content, "_=", 2) == 0)
				ft_putendl_fd("_=/usr/bin/env", STDOUT_FILENO);
			else if (ft_find_char(content, '=') != -1)
				ft_putendl_fd(content, STDOUT_FILENO);
			tmp = tmp->next;
		}
	}
}

void	ft_minishell_core(t_var *var)
{
	if (ft_is_quote_closed(var->line) == EXIT_FAILURE)
	{
		ft_putstr_fd("minishell: error unclosed quote find\n", 2);
		var->status = 2;
		free(var->line);
		return ;
	}
	if (ft_div_by_token(var) == EXIT_FAILURE)
	{
		free(var->line);
		return ;
	}
	ft_command_setup(&var->token);
	ft_set_instru(&var->instru, var->token);
	if (var->instru->next == NULL)
		ft_upgrade_env(var, "_", var->instru->end->token);
	ft_exec(var->instru, var);
	ft_lstclear_instru(&var->instru, &var->token);
	var->token = NULL;
	free(var->line);
}
