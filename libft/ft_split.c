/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 06:41:50 by aelison           #+#    #+#             */
/*   Updated: 2024/08/27 06:41:54 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_all(char **result, unsigned int nb)
{
	unsigned int	i;

	i = 0;
	while (i < nb)
	{
		free(result[i]);
		i = i + 1;
	}
	free(result);
}

static unsigned int	count_c(char const *p1, char c)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	while (p1[i] == c && p1[i] != '\0')
		i++;
	if (p1[i] == '\0')
		return (0);
	count = 1;
	while (p1[i] != '\0')
	{
		if (p1[i] == c && p1[i + 1] != c && p1[i + 1] != '\0')
			count = count + 1;
		i++;
	}
	return (count);
}

static char	*cpy(char const *p, unsigned int start, unsigned int end)
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

static void	split_aux(char **result, char const *s, char c)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	stock;

	i = 0;
	j = 0;
	while (i < count_c(s, c))
	{
		while (s[j] == c && s[j] != '\0')
			j = j + 1;
		if (s[j] != c)
		{
			stock = j;
			while (s[j] != c && s[j] != '\0')
				j = j + 1;
			result[i] = cpy(s, stock, j);
			if (result[i] == NULL)
			{
				free_all(result, count_c(s, c));
				i = count_c(s, c);
			}
		}
		i = i + 1;
	}
	result[i] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char			**result;
	unsigned int	len;

	len = count_c(s, c);
	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
		return (NULL);
	split_aux(result, s, c);
	return (result);
}
