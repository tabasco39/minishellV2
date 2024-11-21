/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:59:05 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/18 10:45:36 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_count_quote(char *str, char *to_count)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (ft_find_char(to_count, str[i]) != -1)
			count++;
		i++;
	}
	return (count);
}

void	ft_check_quote(char *str, int *is_dquote, int *is_squote)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == (char)34)
		{
			i++;
			(*is_dquote)++;
			while (str[i] && str[i] != (char)34)
				i++;
			if (str[i] && str[i] == (char)34)
				(*is_dquote)++;
		}
		else if (str[i] == (char)39)
		{
			i++;
			(*is_squote)++;
			while (str[i] && str[i] != (char)39)
				i++;
			if (str[i] && str[i] == (char)39)
				(*is_squote)++;
		}
		i++;
	}
}

char	*ft_get_to_removes(int is_dq, int is_sq)
{
	char	*result;

	result = malloc(sizeof(char) * 3);
	if (!result)
		return (NULL);
	if ((is_sq != 0 && is_dq != 0) && (is_sq % 2 == 0 && is_dq % 2 == 0))
	{
		free(result);
		return (ft_rm_case(0));
	}
	if (is_dq != 0 && is_dq % 2 == 0)
	{
		free(result);
		return (ft_rm_case(1));
	}
	if (is_sq != 0 && is_sq % 2 == 0)
	{
		result[0] = (char)39;
		result[1] = '\0';
		return (result);
	}
	free(result);
	return (NULL);
}

char	*ft_get_first_quote(char *str)
{
	int		is_squote;
	int		is_dquote;
	char	*result;

	is_dquote = 0;
	is_squote = 0;
	ft_check_quote(str, &is_dquote, &is_squote);
	result = ft_get_to_removes(is_dquote, is_squote);
	return (result);
}
