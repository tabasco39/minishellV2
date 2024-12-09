/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 08:09:13 by aelison           #+#    #+#             */
/*   Updated: 2024/11/25 12:45:45 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_check_double_pipe(char *to_check)
{
	int	i;

	i = 0;
	if (!to_check)
		return (EXIT_SUCCESS);
	while (to_check[i])
	{
		if (to_check[i] == '|')
		{
			i++;
			while (to_check[i])
			{
				while (to_check[i] == ' ')
					i++;
				if (to_check[i] == '|')
					return (EXIT_FAILURE);
				else
					return (EXIT_SUCCESS);
			}
		}
		if (to_check[i])
			i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_is_pipes_closed(char *to_check)
{
	int	len;

	if (!to_check)
		return (EXIT_FAILURE);
	if (to_check && to_check[0] == '\0')
		return (EXIT_FAILURE);
	len = ft_strlen(to_check);
	if (ft_check_double_pipe(to_check) == EXIT_FAILURE)
		return (-1);
	while (--len >= 0)
	{
		if (to_check[len] && to_check[len] == '|')
		{
			len++;
			while (to_check[len])
			{
				if (to_check[len] != ' ')
					return (EXIT_SUCCESS);
				len++;
			}
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

char	*ft_get_quote_arg(char *to_check, int *i)
{
	char	*result;
	char	quote;
	int		start;

	start = *i;
	quote = to_check[*i];
	while (start >= 0)
	{
		if (to_check[start] != '_' && ft_isalnum(to_check[start]) == 0)
			break ;
		start--;
	}
	if (start < 0)
		start = 0;
	(*i)++;
	while (to_check[*i] && to_check[*i] != quote)
		(*i)++;
	result = ft_substr(to_check, start, (*i) - start + 1);
	return (result);
}

char	*ft_get_arg(char *to_check, int *i)
{
	char	*tmp;
	char	*result;
	int		start;

	start = *i;
	while (to_check[*i])
	{
		if (to_check[*i] == '\'' || to_check[*i] == '\"')
			break ;
		(*i)++;
	}
	if (to_check[*i - 1] == '=')
	{
		result = ft_substr(to_check, start, (*i) - start);
		tmp = ft_get_quote_arg(to_check, i);
		result = ft_strjoin_shell(result, tmp);
		free(tmp);
	}
	else
	{
		result = ft_substr(to_check, start, (*i) - start);
		(*i)--;
	}
	return (result);
}

t_list	*ft_divide_all(char *to_divide)
{
	t_list	*result;
	int		i;
	char	*tmp;

	result = NULL;
	i = 0;
	while (to_divide[i])
	{
		if (to_divide[i] == '\'' || to_divide[i] == '\"')
		{
			if (ft_is_open(to_divide, i + 1) == EXIT_SUCCESS)
			{
				tmp = ft_get_quote_arg(to_divide, &i);
				ft_lstadd_back(&result, ft_lstnew(tmp));
			}
		}
		else
		{
			tmp = ft_get_arg(to_divide, &i);
			ft_lstadd_back(&result, ft_lstnew(tmp));
		}
		if (to_divide[i])
			i++;
	}
	return (result);
}
