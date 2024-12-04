/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 08:49:38 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 08:52:43 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*cpy(char *p, unsigned int start, unsigned int end)
{
	char			*result;
	unsigned int	i;

	i = 0;
	if (start == end)
		result = ft_strdup("\0");
	else
	{
		result = (char *)malloc(sizeof(char) * (end - start + 1));
		while (start < end)
		{
			result[i] = p[start];
			i = i + 1;
			start = start + 1;
		}
		result[i] = '\0';
	}
	return (result);
}

char	*ft_utils_split_aux(unsigned int *j, char *s)
{
	unsigned int	stock;
	char			*result;

	stock = *j;
	while (s && s[*j] != '\0')
	{
		if (s[*j] == ' ')
			break ;
		ft_utils_count_c(s, j);
		if (s[*j])
			*j = *j + 1;
	}
	result = cpy(s, stock, *j);
	return (result);
}

void	ft_utils_count_c(char *p1, unsigned int *i)
{
	if (p1[*i] == 39)
	{
		(*i)++;
		while (p1[*i] != 39 && p1[*i])
			(*i)++;
	}
	else if (p1[*i] == 34)
	{
		(*i)++;
		while (p1[*i] != 34 && p1[*i])
			(*i)++;
	}
}
