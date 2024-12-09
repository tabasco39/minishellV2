/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_special_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 06:27:07 by aelison           #+#    #+#             */
/*   Updated: 2024/11/21 10:07:33 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_exec_echo_util(t_token *s, t_token *e, char tmp)
{
	while (s && s != e && (s->command == argument || s->command == dollar))
	{
		if (s->command == argument || s->command == dollar)
		{
			ft_echo(s->token, '\0');
		}
		if (s->next)
		{
			if (s->next->command == argument || s->next->command == dollar)
				ft_putstr_fd(" ", STDOUT_FILENO);
		}
		s = s->next;
	}
	if (s && s == e)
	{
		if ((s->command == argument || s->command == dollar))
			ft_echo(s->token, tmp);
	}
	else
		ft_echo("\0", tmp);
	return (EXIT_SUCCESS);
}

static int	ft_exec_cd_aux(t_var *var, t_token *s, char *home)
{
	char	*path;
	int		result;

	path = NULL;
	if (home)
	{
		if (home[ft_strlen(home) - 1] != '/')
			path = ft_strjoin(home, "/");
		path = ft_strjoin_shell(path, s->token + 1);
		result = ft_cd(path, var, "PWD");
		free(path);
	}
	else
		result = ft_cd(NULL, var, "PWD");
	return (result);
}

int	ft_cd_old_aux(t_var *var, t_token *s, char *old)
{
	int	result;

	result = EXIT_SUCCESS;
	if (old)
	{
		if (s->token[1] == '\0')
		{
			ft_putendl_fd(old + 7, STDOUT_FILENO);
			result = ft_cd(old + 7, var, "PWD");
		}
		else
			result = EXIT_FAILURE;
	}
	else
	{
		ft_putendl_fd("minishell: error OLWD not set", 2);
		result = EXIT_FAILURE;
	}
	return (result);
}

int	ft_exec_cd_util(t_var *var, t_token *s, char *home, char *old)
{
	int	result;

	if (s && s->token[0] == '-')
		result = ft_cd_old_aux(var, s, old);
	else if (s && s->token[0] == '~')
	{
		if (home)
			result = ft_exec_cd_aux(var, s, home + 5);
		else
		{
			ft_putendl_fd("minishell: error HOME not set", 2);
			result = EXIT_FAILURE;
		}
	}
	else
		result = ft_cd(s->token, var, "PWD");
	return (result);
}

void	ft_opt_echo(t_token **start)
{
	int		i;
	t_token	*tmp;

	tmp = *start;
	while (tmp && tmp->command == option)
	{
		if (tmp->token[0] == '-')
		{
			i = 1;
			while (tmp && tmp->token[i] == 'n')
				i++ ;
			if (tmp->token[i] != '\0')
				tmp->command = argument;
		}
		tmp = tmp->next;
	}
}
