/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:45:57 by aelison           #+#    #+#             */
/*   Updated: 2024/11/19 10:54:49 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_is_option(t_token *current, t_token *cmd)
{
	if (current && current->command == option)
	{
		if (cmd && cmd->command != echo)
		{
			ft_putstr_fd("minishell: builtin error no option\n", 2);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	ft_exec_echo(t_token *start, t_token *end)
{
	int		result;
	char	tmp;

	tmp = '\n';
	if (start && start->command == option)
	{
		ft_opt_echo(&start);
		while (start && start->command == option)
			start = start->next;
		if (!start)
			tmp = '\0';
		if (start && start->prev->command == option)
			tmp = '\0';
	}
	result = ft_exec_echo_util(start, end, tmp);
	return (result);
}

int	ft_exec_cd(t_var *var, t_token *start)
{
	char	*tmp;
	char	*old;
	int		result;

	result = EXIT_SUCCESS;
	old = ft_getvar(var->env, "OLDPWD");
	tmp = ft_getvar(var->env, "HOME");
	if (ft_handle_many_arg(start))
		return (EXIT_FAILURE);
	else if (start)
		result = ft_exec_cd_util(var, start, tmp, old);
	else if (!start)
	{
		if (tmp)
			result = ft_cd(tmp + 5, var, "PWD");
		else
		{
			ft_putendl_fd("minishell: error HOME not set", 2);
			result = EXIT_FAILURE;
		}
	}
	return (result);
}

int	ft_exec_pwd(void)
{
	int	result;

	if (ft_pwd() != NULL)
		result = EXIT_SUCCESS;
	else
		result = EXIT_FAILURE;
	return (result);
}

void	ft_exec_exit(t_var *var, t_token *start, t_token *end)
{
	int			i;
	int			status;
	long long	value;

	i = 0;
	if ((start && start->prev->prev) || end->next)
		return ;
	status = EXIT_SUCCESS;
	if (start == NULL)
		ft_exit(var, var->status, EXIT_SUCCESS);
	else if (start && start->next)
	{
		if (start->next->command == argument)
		{
			ft_putendl_fd("minishell: too many arg", 2);
			var->status = 1;
		}
	}
	else
	{
		value = ft_atoi_shell(start->token, &status);
		ft_exit_status(start, var, &status, &value);
	}
}
