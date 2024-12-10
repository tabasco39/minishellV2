/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 07:53:00 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/18 10:49:11 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_exit_status(t_token *start, int *status,
		long long *value, int do_exit)
{
	t_var	*var;

	var = ft_get_struct_var();
	if (ft_valid_exit(start->token, *value) == EXIT_FAILURE)
	{
		*status = EXIT_FAILURE;
		ft_putendl_fd("minishell: error numeric arg required", 2);
	}
	if (*value < 0 || *value > 255)
		*value = *value % 256;
	if (do_exit == EXIT_SUCCESS)
	{
		if (*status == EXIT_FAILURE)
			ft_exit(var, 2);
		ft_exit(var, *value);
	}
	else
	{
		if (*status == EXIT_FAILURE)
			var->status = 2;
		else
			var->status = *value;
	}
}

int	ft_handle_oldpwd(char *to_change, t_var *var, char *tmp, int show)
{
	int		len;
	int		result;
	char	*oldpwd;
	char	current[1014];

	result = 0;
	getcwd(current, 1024);
	oldpwd = tmp + ft_find_char(tmp, '=') + 1;
	len = ft_max_value(ft_strlen(current), ft_strlen(to_change));
	if (ft_strncmp(current, to_change, len) != 0)
	{
		if (ft_strncmp(to_change, oldpwd, len) != 0)
			ft_upgrade_env(var, "OLDPWD", to_change);
		if (ft_getvar(var->env, "PWD") == NULL && show == 0)
		{
			result = 1;
			ft_unset_envp(var, "OLDPWD");
			ft_export_aux(var, "OLPWD");
		}
	}
	return (result);
}

void	ft_quote_rdefine(t_token **start, t_token *end)
{
	t_token	*tmp;
	char	*new_str;
	char	*q_ref;

	tmp = *start;
	while (tmp && tmp != end)
	{
		q_ref = ft_get_first_quote(tmp->token);
		new_str = ft_del_quote(tmp->token, q_ref);
		free(tmp->token);
		tmp->token = new_str;
		tmp = tmp->next;
		free(q_ref);
	}
	if (tmp && tmp == end)
	{
		q_ref = ft_get_first_quote(tmp->token);
		new_str = ft_del_quote(tmp->token, q_ref);
		free(tmp->token);
		tmp->token = new_str;
		free(q_ref);
	}
}

t_token	*ft_define_first_arg(t_token *start, t_token *end)
{
	t_token	*result;

	result = NULL;
	if (start != end)
	{
		result = start->next;
		if (start->command != echo)
			ft_quote_rdefine(&result, end);
	}
	return (result);
}

int	ft_handle_many_arg(t_token *start)
{
	if (start && start->next && (start->next->command == argument
			|| start->next->command == dollar))
	{
		ft_putendl_fd("minishell: error cd too many arg", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
