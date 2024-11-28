/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 06:59:12 by aelison           #+#    #+#             */
/*   Updated: 2024/11/19 12:16:47 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void	ft_echo(char *to_print, char option)
{
	ft_putstr_fd(to_print, STDOUT_FILENO);
	if (option == '\n')
		ft_putchar_fd('\n', STDOUT_FILENO);
}

void	ft_oldpwd(t_var *var, char *current, char *to_change)
{
	char		*tmp;
	int			len;
	static int	show;

	if (!current || !to_change)
		return ;
	len = ft_max_value(ft_strlen(current), ft_strlen(to_change));
	tmp = ft_getvar(var->env, "OLDPWD");
	if (ft_getvar(var->env, "PWD") != NULL)
		show = 1;
	if (tmp)
		show = ft_handle_oldpwd(to_change, var, tmp, show);
	else
	{
		tmp = ft_strjoin("OLDPWD=", to_change);
		ft_export_aux(var, tmp);
		free(tmp);
	}
}

char	*ft_pwd(void)
{
	char	directory[1024];
	char	*result;

	result = getcwd(directory, 1024);
	if (result == NULL)
	{
		ft_putstr_fd("minishell: error: getcwd fail\n", 2);
		return (result);
	}
	ft_putendl_fd(directory, STDOUT_FILENO);
	return (result);
}

int	ft_cd(char *path, t_var *var, char *var_name)
{
	char	buff[1024];
	char	to_change[1024];
	char	*result;

	result = getcwd(to_change, 1024);
	if (access(path, F_OK) != 0)
	{
		ft_putendl_fd("minishell: cd: No such file directory", 2);
		return (EXIT_FAILURE);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putendl_fd("minishell: cd: Permission denied", 2);
		return (EXIT_FAILURE);
	}
	if (chdir(path) == -1)
	{
		ft_putendl_fd("minishell: cd: No such file directory", 2);
		return (EXIT_FAILURE);
	}
	ft_upgrade_env(var, var_name, getcwd(buff, 1024));
	ft_oldpwd(var, getcwd(buff, 1024), result);
	return (EXIT_SUCCESS);
}
