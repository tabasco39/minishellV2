/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 09:43:40 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/28 08:41:23 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_execve(t_instru *tmp, char **all_path, char **params, t_var *var)
{
	char	*path;

	path = NULL;
	if (all_path)
		path = ft_handle_error_dir(all_path, params, var);
	else
		path = ft_handle_no_such_file(tmp, all_path, params, var);
	if (!path)
	{
		ft_free_minishell(var);
		ft_free_all(all_path);
		ft_free_all(params);
		ft_putendl_fd("minishell : command not found", 2);
		exit(127);
	}
	ft_free_all(all_path);
	if (execve(path, params, var->tab_env) == -1)
		exit(EXIT_FAILURE);
}

void	ft_exec_path(t_instru *tmp, t_var *var)
{
	char	**params;
	char	**all_path;

	if (tmp->start->command == not_comm)
	{
		ft_putendl_fd("minishell: syntax error", 2);
		ft_free_minishell(var);
		exit(2);
	}
	if (tmp->start->command >= env && tmp->start->command <= e_exit)
		var->status = ft_exec_builtin(var, tmp->start, tmp->end);
	if (!(tmp->start->command >= env && tmp->start->command <= e_exit)
		&& (!(tmp->start->command >= redirect_input
				&& tmp->start->command <= append_redirect_output)))
	{
		params = ft_get_execve_parameter(tmp);
		all_path = ft_get_all_path(var->env, params[0]);
		ft_execve(tmp, all_path, params, var);
	}
	ft_free_minishell(var);
	exit(var->status);
}

pid_t	ft_exec_current_instru(t_instru *tmp, t_exec *it, int input_fd,
		t_var *var)
{
	int		has_redirection;
	pid_t	pid;
	t_token	*target;

	var->current_status = 0;
	ft_init_exec_current_instru(&pid, &has_redirection);
	target = ft_find_cmd_token(tmp);
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		target = ft_find_cmd_token(tmp);
		ft_dup_fd_not_start_instru(*it, &input_fd);
		if (target != NULL)
		{
			ft_handle_redirection(var, target,
				&it->here_doc_fd[0], &it->redir_in_fd);
			has_redirection = 1;
		}
		ft_dup_fd_end_instru(*it, has_redirection);
		ft_handle_empty_com(tmp, var);
		ft_exec_path(tmp, var);
	}
	signal(SIGQUIT, SIG_IGN);
	return (pid);
}

pid_t	ft_exec_all_instru(t_instru *tmp, t_exec *iteration, int input_fd,
		t_var *var)
{
	pid_t	pid;

	pid = 0;
	while (tmp)
	{
		if (ft_exec_once(tmp, var) == EXIT_SUCCESS)
			return (ft_exec_once_builtin(tmp, var, &pid));
		if (iteration->i != iteration->end)
		{
			if (pipe(iteration->pipefd) == -1)
				return (pid);
		}
		pid = ft_exec_current_instru(tmp, iteration, input_fd, var);
		if (iteration->i != iteration->start)
			close(input_fd);
		if (iteration->i != iteration->end)
		{
			if (iteration->pipefd[1] != -1)
				close(iteration->pipefd[1]);
			input_fd = iteration->pipefd[0];
		}
		iteration->i++;
		tmp = tmp->next;
	}
	return (pid);
}

void	ft_exec(t_instru *tmp, t_var *var)
{
	t_token	*target;
	int		input_fd;
	pid_t	pid;

	target = NULL;
	input_fd = STDIN_FILENO;
	ft_init_exec(var->iteration, var);
	if (ft_handle_unclosed_pipe(var, &tmp, var->iteration) == EXIT_FAILURE)
		return ;
	if (ft_init_pipe_hd(tmp, var) == EXIT_FAILURE)
		return ;
	if (ft_check_before_exec(tmp, target, var->iteration, var) == EXIT_FAILURE)
		return ;
	pid = ft_exec_all_instru(tmp, var->iteration, input_fd, var);
	if (var->iteration->redir_in_fd != -1 && var->iteration->redir_in_fd != 0)
		close(var->iteration->redir_in_fd);
	ft_close_pipe(var);
	waitpid(pid, &var->current_status, 0);
	ft_handle_exit_status(var);
	tmp = var->instru;
	while (tmp)
	{
		wait(NULL);
		tmp = tmp->next;
	}
}
