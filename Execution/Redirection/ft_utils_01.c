/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_01.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:14:22 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/25 09:08:42 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_redir_error(char *check_ambigous, t_token *target, int *output_fd)
{
	void	*dir;
	t_var	*var;

	var = ft_get_struct_var();
	ft_check_access_directory(check_ambigous, target);
	dir = opendir(target->next->token);
	if (dir)
	{
		closedir(dir);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(target->next->token, 2);
		ft_putendl_fd(": is a directory ", 2);
		ft_free_minishell(var);
		return (EXIT_FAILURE);
	}
	if (dup2(*output_fd, STDOUT_FILENO) == -1)
	{
		perror("ERROR FOR DUP\n");
		return (EXIT_FAILURE);
	}
	close(*output_fd);
	return (EXIT_SUCCESS);
}

int	ft_is_ambigous(t_comm type, char *to_check, int start)
{
	int		result;
	int		i;
	char	q_ref;

	result = 0;
	q_ref = '\0';
	i = start;
	while (i >= 0)
	{
		if (to_check[i] == (char)34 || to_check[i] == (char)39)
		{
			q_ref = to_check[i];
			break ;
		}
		i--;
	}
	i = start;
	if (i >= 0 && q_ref != '\0'
		&& ft_is_open(to_check, start) == EXIT_FAILURE)
		result = 1;
	if (type == dollar && q_ref == '\0')
		result = 1;
	return (result);
}

void	ft_free_char(char **tmp)
{
	if (*tmp)
		free(*tmp);
	exit(EXIT_FAILURE);
}

int	ft_free_in_ambigous(t_var *var, char *check_ambigous)
{
	if (check_ambigous == NULL)
	{
		ft_free_minishell(var);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
