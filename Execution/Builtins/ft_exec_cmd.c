/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:54:10 by aelison           #+#    #+#             */
/*   Updated: 2024/11/21 08:52:09 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_valid_exit(char *ref, long long value)
{
	char	*tmp;
	char	*tmp_ref;
	char	*to_cmp;

	tmp = ft_itoa_shell(value);
	tmp_ref = ft_strtrim(ref, " ");
	to_cmp = ft_strtrim(tmp_ref, "+");
	free(tmp_ref);
	if (ft_strncmp(to_cmp, tmp, ft_strlen(to_cmp)) != 0)
	{
		free(tmp);
		free(to_cmp);
		return (EXIT_FAILURE);
	}
	free(to_cmp);
	free(tmp);
	return (EXIT_SUCCESS);
}

char	ft_first_quote(char *word, char first, char second)
{
	int	i;

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

void	ft_lstclear_instru(t_instru **instru, t_token **head)
{
	t_instru	*tmp;

	ft_lstclear_shell(head);
	while (instru && *instru)
	{
		tmp = *instru;
		*instru = (*instru)->next;
		free(tmp);
	}
}

void	ft_exit(t_var *var, long long end)
{
	ft_free_minishell(var);
	ft_putendl_fd("exit : good bye", 1);
	rl_clear_history();
	exit(end);
}

int	ft_exec_builtin(t_var *all_var, t_token *start, t_token *end)
{
	int		result;
	t_token	*new_s;

	result = EXIT_FAILURE;
	new_s = ft_define_first_arg(start, end);
	if (ft_is_option(new_s, start) == EXIT_FAILURE)
		return (result);
	if (start->command == echo)
		result = ft_exec_echo(new_s, end);
	else if (start->command == cd)
		result = ft_exec_cd(all_var, new_s);
	else if (start->command == pwd)
		result = ft_exec_pwd();
	else if (start->command == e_export)
		result = ft_exec_export(all_var, new_s, end);
	else if (start->command == unset)
		result = ft_exec_unset(all_var, new_s, end);
	else if (start->command == env)
		result = ft_exec_env(all_var, new_s);
	else if (start->command == e_exit)
		result = ft_exec_exit(all_var, start, new_s);
	all_var->status = result;
	ft_close_pipe(all_var);
	return (result);
}
