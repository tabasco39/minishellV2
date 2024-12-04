/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_03.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:27:37 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/28 08:41:36 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_handle_no_such_file(t_instru *tmp, char **all_path, char **params,
		t_var *var)
{
	char	buffer[1024];
	char	*result;

	result = NULL;
	getcwd(buffer, 1024);
	result = ft_strjoin(buffer, "/");
	result = ft_strjoin_shell(result, tmp->start->token);
	if (access(result, X_OK) != 0)
	{
		free(result);
		ft_free_all(params);
		ft_free_all(all_path);
		ft_free_minishell(var);
		ft_putendl_fd("minishell : No such file or directory", 2);
		exit(127);
	}
	return (result);
}

void	ft_errno(char **params, char **all_path,
				char *message, int status)
{
	t_var	*var;

	var = ft_get_struct_var();
	ft_free_all(params);
	ft_free_all(all_path);
	ft_free_minishell(var);
	ft_putstr_fd("minishell : ", 2);
	ft_putendl_fd(message, 2);
	exit(status);
}

char	*ft_handle_error_dir(char **all_path, char **params, t_var *var)
{
	char	*result;
	void	*dir;

	result = NULL;
	var = (void *) var;
	if (all_path[1] == NULL)
	{
		dir = opendir(all_path[0]);
		if (dir)
		{
			closedir(dir);
			ft_errno(params, all_path, "Is a directory", 126);
		}
		if (ft_check_dir(all_path[0]) == EXIT_FAILURE)
			ft_errno(params, all_path, "Not a directory", 126);
		else if (access(all_path[0], X_OK) == -1)
		{
			if (errno == EACCES)
				ft_errno(params, all_path, "Permission denied", 126);
			else
				ft_errno(params, all_path, "No such file or directory", 127);
		}
	}
	result = ft_verify_exec_cmd(all_path);
	return (result);
}

void	ft_handle_empty_com(t_instru *tmp, t_var *var)
{
	if (tmp->start->token[0] == '\0')
	{
		ft_display_error_without_exit("command not found", "", 127, var);
		ft_close_pipe(var);
		ft_free_minishell(var);
		exit(127);
	}
}

int	ft_init_pipe_hd(t_instru *tmp, t_var *var)
{
	if (tmp && tmp->start)
	{
		if (ft_find_cmd(tmp->start, heredoc) != NULL)
		{
			if (pipe(var->iteration->here_doc_fd) == -1)
				return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
