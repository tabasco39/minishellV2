/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 09:43:40 by aranaivo          #+#    #+#             */
/*   Updated: 2024/10/10 09:33:59 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


//commande validation ==> exit if redirection error // => else ignore 

static char	**ft_get_execve_parameter(t_instru *current)
{
	t_token	*tmp;
	char	**result;
	int		i;

	i = 0;
	tmp = current->start;
	result = malloc(sizeof(char *) * (ft_count_token_in_instru(current) + 1));
	while (i < ft_count_token_in_instru(current))
	{
		result[i] = tmp->token;
		if (tmp->command >= redirect_input && tmp->command <= append_redirect_output)
		{
			result[i] = NULL;
			return (result);
		}
		i++;
		tmp = tmp->next;
	}
	result[i] = NULL;
	return (result);
}

static int	ft_count_special_char(t_token *token, t_comm to_count)
{
	t_token *tmp;
	int		count;

	tmp = token;
	count = 0;
	while (tmp)
	{
		if (tmp->command == to_count)
			count++;
		tmp = tmp->next;
	}
	return (count);
}


static t_token *ft_find_cmd_token(t_instru *instru)
{
	t_token *result;
	int		i;

	result = instru->start;
	i = ft_count_token_in_instru(instru);
	while (i > 0)
	{
		if (result->command >= redirect_input && result->command <= append_redirect_output)
			return (result);
		i--;
		result = result->next;
	}
	return (NULL);
}

t_token *ft_find_cmd(t_token *start, t_comm to_find)
{
	t_token *tmp;

	tmp = start;
	while (tmp)
	{
		if (tmp->command == to_find)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*ft_check_ambigous(t_var *var, char *token)
{
	char *result;
	char *tmp;

	tmp = ft_strdup(token);
	if (token[0] == '$')
	{
		tmp = ft_strdup(token);
		result = ft_parse_final(var, token);
		if (ft_find_char(result, ' ') != -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(tmp, 2);
			ft_putendl_fd(": ambigous redirect", 2);
			free(result);
			free(tmp);
			return (NULL);
		}
		else
		{
			free(tmp);
			return (result);
		}
	}
	return (token);
}

void ft_handle_redirection(t_var *var, t_token *target, int *input_fd, int *output_fd, int *has_redirection, int hd)
{
	char	*check_ambigous;

	*has_redirection = 1;
	if (target->next == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putendl_fd("No such file or directory", 2);
		exit(EXIT_FAILURE);
	}
	if ( access(target->next->token, W_OK) == -1 && target->command != delimiter_redirect_input)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(target->next->token, 2);
		ft_putendl_fd(": Permission denied ", 2);
		exit(EXIT_FAILURE);
	}
	check_ambigous = ft_check_ambigous(var,target->next->token);
	while (target->is_end != 1)
	{
		if ((target->next->command >= e_pipe && target->next->command <= append_redirect_output))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("syntax error near unexpected token ", 2);
			ft_putendl_fd(target->next->token, 2);
			exit(EXIT_FAILURE);
		}
		if (target->command == redirect_output)
		{
			if ( check_ambigous == NULL)
				exit(EXIT_FAILURE);
			else
				target->next->token = check_ambigous;
			*output_fd = open(target->next->token, O_CREAT | O_RDWR | O_TRUNC , S_IRWXU);
			dup2(*output_fd, STDOUT_FILENO);
			close(*output_fd);
		}
		if (target->command == append_redirect_output)
		{
			if ( check_ambigous == NULL)
				exit(EXIT_FAILURE);
			else
				target->next->token = check_ambigous;
			*output_fd = open(target->next->token, O_CREAT | O_RDWR | O_APPEND , S_IRWXU);
			dup2(*output_fd, STDOUT_FILENO);
			close(*output_fd);
		}
		if (target->command == redirect_input)
		{
			if (ft_valid_redirect_input(target) == EXIT_FAILURE)
				exit(EXIT_FAILURE);
			*input_fd = open(target->next->token,O_RDONLY);
			dup2(*input_fd, STDIN_FILENO);
		}
		if (target->command == delimiter_redirect_input)
		{
			dup2(hd, STDIN_FILENO);
			close(hd);
		}
		target = target->next;
	}
}


void	ft_simul_heredoc(t_token *target, int hd, t_var *var)
{
	char	*here_doc_result;
	char 	buffer[1024];
	pid_t	pid;

	here_doc_result = ft_strdup_shell("");
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		while (target)
		{
			if (target->command == delimiter_redirect_input)
			{
				while (1)
				{
					ft_putstr_fd(">", STDOUT_FILENO);
					int bytes_read = read(STDIN_FILENO, buffer, 1023);
					if (bytes_read > 1)
						buffer[bytes_read - 1] = '\0';
					else
						buffer[bytes_read] = '\0';
					if (ft_strncmp(target->next->token, buffer, ft_strlen(buffer)) != 0)
					{
						if (bytes_read > 1)
						{
							here_doc_result = ft_strjoin_shell(here_doc_result, buffer);
							here_doc_result = ft_strjoin_shell(here_doc_result, "\n");
						}
						else
							here_doc_result = ft_strjoin_shell(here_doc_result, buffer);
					}
					else
						break;
				}
				if (ft_find_cmd(target->next, delimiter_redirect_input) != NULL)
				{
					free(here_doc_result);
					here_doc_result = ft_strdup_shell("");
				}
			}
			target = target->next;
		}
		here_doc_result = ft_parse_final(var, here_doc_result);
		if (ft_find_char(here_doc_result, '\n') == -1 )
			ft_putendl_fd(here_doc_result, hd);
		else
			ft_putstr_fd(here_doc_result, hd);	
		exit(EXIT_SUCCESS);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, NULL, 0);
}

void ft_exec_path(t_instru *tmp, t_var *var)
{
	char		**params;
	char		**all_path;
	char		*path;

	if (!(tmp->start->command >= env && tmp->start->command <= e_exit))
	{
		params = ft_get_execve_parameter(tmp);
		all_path = ft_get_all_path(var->env, params[0]);
		path	= ft_verify_exec_cmd(all_path);
		if (execve(path, params, var->tab_env) == -1)
			exit(EXIT_FAILURE);
	}
	else
	{
		exit(EXIT_SUCCESS);
	}
}
void ft_init_exec_current_instru(pid_t *pid, int *output_fd, int *has_redirection)
{
	*pid = fork();
	*output_fd = -1;
	*has_redirection = 0;
}

pid_t ft_exec_current_instru(t_instru *tmp, t_exec it, int input_fd, t_var *var)
{
	pid_t 		pid;
	t_token		*target;
	int 		output_fd;
	int 		has_redirection;

	ft_init_exec_current_instru(&pid, &output_fd, &has_redirection);
	target = ft_find_cmd_token(tmp);
	if (pid == 0)
	{
		target = ft_find_cmd_token(tmp);
		if (it.i != it.start)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (target != NULL)
			ft_handle_redirection(var, target, &input_fd, &output_fd, &has_redirection, it.here_doc_fd[0]);
		if (it.i != it.end && has_redirection == 0)
		{
			dup2(it.pipefd[1] , STDOUT_FILENO);
			close(it.pipefd[0]);
			close(it.pipefd[1]);
		}
		ft_exec_path(tmp, var);
	}
	if (target)
		waitpid(pid, NULL, 0);
	return (pid);
}

pid_t ft_exec_all_instru(t_instru *tmp, t_exec iteration, int input_fd, t_var *var)
{
	pid_t 	pid;

	while (tmp)
	{
		// if (tmp->start->command == e_export || tmp->start->command == unset)
		// {
		// 	if (ft_exec_builtin(var, tmp->start, tmp->end) == EXIT_FAILURE)
		// 		return;
		// }
		if (tmp->next == NULL)
			ft_exec_builtin(var, tmp->start, tmp->end);
		if (iteration.i != iteration.end)
			pipe(iteration.pipefd);
		pid = ft_exec_current_instru(tmp, iteration, input_fd,var);
		if (iteration.i != iteration.start)
			close(input_fd);
		if (iteration.i != iteration.end)
		{
			close(iteration.pipefd[1]);
			input_fd = iteration.pipefd[0];
		}
		iteration.i++;
		tmp = tmp->next;
	}
	return (pid);
}

void ft_init_exec(t_exec *it, t_var *var)
{
	it->start = 0;
	it->i = 0;
	it->end = ft_count_special_char(var->token, e_pipe);
}

void	ft_exec(t_instru *tmp, t_var *var)
{
	t_token		*target;
	t_exec		iteration;
	int			input_fd;
	pid_t 		pid;

	target = NULL;
	input_fd = STDIN_FILENO;
	pipe(iteration.here_doc_fd);
	ft_init_exec(&iteration, var);
	if (tmp)
		target = ft_find_cmd(tmp->start, delimiter_redirect_input);
	if (target)
	{
		ft_simul_heredoc(target, iteration.here_doc_fd[1], var);
		close(iteration.here_doc_fd[1]);
	}
	pid = ft_exec_all_instru(tmp, iteration, input_fd, var);
	waitpid(pid, NULL, 0);
	tmp = var->instru;
	while (tmp)
	{
		wait(NULL);
		tmp = tmp->next;
	}
}

