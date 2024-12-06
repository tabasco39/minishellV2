/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_02.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:10:55 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/29 11:40:20 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

pid_t	ft_exec_once_builtin(t_instru *tmp, t_var *var, pid_t *pid)
{
	var->status = ft_exec_builtin(var, tmp->start, tmp->end);
	*pid = fork();
	if (*pid == 0)
	{
		ft_free_minishell(var);
		exit(var->status);
	}
	return (*pid);
}

void	ft_init_exec_current_instru(pid_t *pid)
{
	t_var	*var;

	var = ft_get_struct_var();
	*pid = fork();
	var->iteration->check = 0;
}

int	ft_check_dir(char *path)
{
	int		len;
	char	*tmp;
	void	*is_dir;

	len = ft_strlen(path);
	len--;
	while (path[len] && path[len] != '/')
		len--;
	if (len > 0 && path[len])
	{
		tmp = ft_substr(path, 0, len);
		is_dir = opendir(tmp);
		if (is_dir == NULL)
		{
			free(tmp);
			if (errno == EACCES)
				return (EXIT_SUCCESS);
			return (EXIT_FAILURE);
		}
		free(tmp);
		closedir(is_dir);
	}
	return (EXIT_SUCCESS);
}

int	ft_exec_once(t_instru *tmp, t_var *var)
{
	if (tmp->start->command >= env && tmp->start->command <= e_exit)
	{
		if (ft_find_cmd(var->token, redirect_output) == NULL
			&& ft_find_cmd(var->token, redirect_input) == NULL
			&& ft_find_cmd(var->token, append_redirect_output) == NULL
			&& ft_find_cmd(var->token, heredoc) == NULL
			&& ft_find_cmd(var->token, e_pipe) == NULL)
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

void	ft_check_point(t_instru *tmp, t_var *var)
{
	size_t	len_token;

	len_token = ft_strlen(tmp->start->token);
	if (ft_strncmp(tmp->start->token, ".", ft_max_value(len_token, 1)) == 0
		&& tmp->start->command == in_sys)
	{
		ft_putendl_fd("minishell test: filename argument required", 2);
		ft_free_minishell(var);
		exit (2);
	}
	if (ft_strncmp(tmp->start->token, "..", ft_max_value(len_token, 2)) == 0
		&& tmp->start->command == in_sys)
	{
		ft_putendl_fd("minishell : command not found", 2);
		ft_free_minishell(var);
		exit (127);
	}
}
