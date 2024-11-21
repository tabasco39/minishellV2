/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 10:55:02 by aelison           #+#    #+#             */
/*   Updated: 2024/11/21 13:11:45 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void	ft_display_command(t_comm cmd)
{
	printf("t_comm = ");
	if (cmd == argument)
		printf("argument");
	else if (cmd == option)
		printf("option");
	else if (cmd == env)
		printf("env");
	else if (cmd == pwd)
		printf("pwd");
	else if (cmd == cd)
		printf("cd");
	else if (cmd == echo)
		printf("echo");
	else if (cmd == unset)
		printf("unset");
	else if (cmd == question)
		printf("question");
	else if (cmd == e_export)
		printf("export");
	else if (cmd == e_exit)
		printf("exit");
	else if (cmd == e_pipe)
		printf("pipe");
	else if (cmd == redirect_input)
		printf("redirect_input : <");
	else if (cmd == redirect_output)
		printf("redirect_output : >");
	else if (cmd == heredoc)
		printf("delimiter_redir_input : <<");
	else if (cmd == append_redirect_output)
		printf("append_redir_output : >>");
	else if (cmd == not_comm)
		printf("not_comm");
	else if (cmd == in_sys)
		printf("in_sys");
	else if (cmd == dollar)
		printf("dollar");
	printf("\n\n");
}

void	ft_display_token(t_token *token)
{
	int	i;

	i = 0;
	printf("\n=================== Display token ==========================\n");
	while (token)
	{
		printf("Token %d\n", i);
		printf("token = %s\t", token->token);
		printf("is_head = %d\t", token->is_head);
		printf("is_end = %d\t", token->is_end);
		ft_display_command(token->command);
		i++;
		token = token->next;
	}
}

void	ft_display_instru(t_instru *head)
{
	t_token *start;
	t_token *end;

	printf("\n=================== Display INSTRU ==========================\n");
	while (head)
	{
		printf("\n========= instr %d ===============\n", head->id);
		start = head->start;
		end = head->end;
		while (start != end)
		{
			printf("\ntoken = %s\t", start->token);
			printf("is_head = %d\t", start->is_head);
			printf("is_end = %d\t", start->is_end);
			ft_display_command(start->command);
			start = start->next;
			if (start == end)
			{
				printf("token = %s\t", start->token);
				printf("is_head = %d\t", start->is_head);
				printf("is_end = %d\t", start->is_end);
				ft_display_command(start->command);
				printf("\n");
			}
		}
		head = head->next;
	}
}

void	ft_disp_dchar(char **str)
{
	int	i;

	i = 0;
	printf("\n===================== Display char ** ======================\n");
	if (str && *str)
	{
		while (str[i] != NULL)
		{
			printf("str[%d] = %s\n", i, str[i]);
			i++;
		}
	}
}
*/

void	ft_init_bash_history(char *home)
{
	char	*line;
	char	*dir;
	int		fd;

	line = "\0";
	dir = ft_strjoin(home, "/");
	dir = ft_strjoin_shell(dir, ".bash_history");
	fd = open(dir, O_RDONLY);
	free(dir);
	if (fd != -1)
	{
		while (line != NULL)
		{
			line = get_next_line(fd);
			if (line)
			{
				add_history(line);
				free(line);
			}
		}
	}
	close(fd);
}

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

int	ft_top_level_check(t_var *var)
{
	if (ft_is_quote_closed(var->line) == EXIT_FAILURE)
	{
		ft_putstr_fd("minishell: error unclosed quote find\n", 2);
		var->status = 2;
		free(var->line);
		return (EXIT_FAILURE);
	}
	if (ft_div_by_token(var) == EXIT_FAILURE)
	{
		free(var->line);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	ft_debug(t_var *var)
{
	if (ft_top_level_check(var) == EXIT_FAILURE)
		return ;
	ft_command_setup(&var->token);
	ft_parse(var->token);
	ft_set_instru(&var->instru, var->token);
	if (var->instru->next == NULL)
		ft_upgrade_env(var, "_", var->instru->end->token);
	if (var->token->command == not_comm
		|| ft_valid_redir(var->token) == EXIT_FAILURE)
	{
		ft_display_error_without_exit("syntax error near unexpected token : ",
			var->token->token, 2, var);
	}
	else
		ft_exec(var->instru, var);
	ft_lstclear_instru(&var->instru, &var->token);
	var->token = NULL;
	free(var->line);
}
