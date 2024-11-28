/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 09:15:53 by aelison           #+#    #+#             */
/*   Updated: 2024/11/27 13:34:13 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_atoi_shell_aux(char *str, long long *sign)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] == '-')
	{
		*sign = -1;
		i++;
	}
	return (i);
}

long long	ft_atoi_shell(char *str, int *status)
{
	long long	result;
	long long	sign;
	int			i;

	*status = EXIT_SUCCESS;
	result = 0;
	sign = 1;
	i = ft_atoi_shell_aux(str, &sign);
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == 0 && str[i] != 32)
		{
			*status = EXIT_FAILURE;
			return (0);
		}
		if (ft_isdigit(str[i]) != 0)
			result = (result * 10) + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	ft_pipe_aux(char *to_check)
{
	int	i;

	i = 0;
	while (to_check[i] >= (char)9 && to_check[i] <= (char)13)
		i++;
	if (to_check[i] == '\0')
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	*ft_pipes_valid(t_var *var, char *line)
{
	char	*to_add;
	char	*tmp;

	while (ft_is_pipes_closed(line) == EXIT_FAILURE)
	{
		to_add = readline(CYAN"PIPE> "RESET);
		ft_rl_cut(var, to_add);
		if (ft_is_pipes_closed(to_add) == -1)
		{
			ft_putendl_fd("syntax error near '|' ", 2);
			return (line);
		}
		if (to_add && to_add[0] != '\0' && ft_pipe_aux(to_add) == EXIT_SUCCESS)
		{
			tmp = ft_strtrim(to_add, " \n\t\v\f\r");
			to_add = tmp;
			line = ft_strjoin_shell(line, " ");
			line = ft_strjoin_shell(line, to_add);
			add_history(line);
			free(to_add);
		}
	}
	return (line);
}

int	ft_count_special_char(t_token *token, t_comm to_count)
{
	t_token	*tmp;
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
