/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 11:04:46 by aelison           #+#    #+#             */
/*   Updated: 2024/11/19 12:06:51 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_exec_env(t_var *var, t_token *nxt)
{
	char	buffer[1024];

	if (nxt)
	{
		ft_putendl_fd("minishell: builtins env error: nor arg", 2);
		return (EXIT_FAILURE);
	}
	getcwd(buffer, 1024);
	if (var && var->env)
	{
		if (ft_getvar(var->env, "PWD") != NULL)
			ft_upgrade_env(var, "PWD", buffer);
		ft_display_env(&var->env, "success");
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	ft_exec_export(t_var *var, t_token *start, t_token *end)
{
	int	result;

	result = EXIT_FAILURE;
	if (start && ft_strncmp(start->token, "", 1) == 0)
	{
		ft_putstr_fd("minishell: export : not a valid identifier\n", 2);
		return (result);
	}
	while (start != end && start)
	{
		ft_export_aux(var, start->token);
		start = start->next;
	}
	if (start)
		result = ft_export_aux(var, start->token);
	else
		result = ft_export_aux(var, NULL);
	return (result);
}

int	ft_valid_export(char *to_add)
{
	int		i;

	i = 1;
	if (to_add[0] != '_' && ft_isalpha(to_add[0]) == 0)
	{
		ft_putendl_fd("minishell: error export not valid key", 2);
		return (EXIT_FAILURE);
	}
	while (to_add[i] != '=')
	{
		if (to_add[i] != '_' && ft_isalnum(to_add[i]) == 0)
		{
			ft_putendl_fd("minishell: error export not valid key", 2);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_exec_unset(t_var *var, t_token *start, t_token *end)
{
	int	result;

	result = EXIT_SUCCESS;
	if (start == NULL)
		return (result);
	else
	{
		while (start != end && start)
		{
			ft_unset_envp(var, start->token);
			start = start->next;
		}
		if (start)
			result = ft_unset_envp(var, start->token);
		else
			ft_putchar_fd('\n', 1);
	}
	return (result);
}
