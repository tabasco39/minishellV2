/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 10:30:14 by aelison           #+#    #+#             */
/*   Updated: 2024/10/14 07:23:03 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_max_value(int val1, int val2)
{
	if (val1 > val2)
		return (val1);
	return (val2);
}

int	ft_is_quote_closed(char *to_check)
{
	int		i;
	int		status;
	char	quote;

	i = 0;
	status = EXIT_SUCCESS;
	while (to_check[i] != '\0')
	{
		if (status == EXIT_SUCCESS)
		{
			if (to_check[i] == (char)34 || to_check[i] == (char)39)
			{
				quote = to_check[i];
				status = EXIT_FAILURE;
			}
		}
		else if (status == EXIT_FAILURE)
		{
			if (to_check[i] == quote)
				status = EXIT_SUCCESS;
		}
		i++;
	}
	return (status);
}

int	ft_find_char(char *token, char to_find)
{
	int	result;

	result = 0;
	if (!token || to_find == '\0')
		return (-1);
	while (token[result] != '\0')
	{
		if (token[result] == to_find)
			return (result);
		result++;
	}
	return (-1);
}

char	*ft_strdup_shell(char *s)
{
	unsigned int	len;
	char			*result;
	unsigned int	i;

	i = 0;
	len = ft_strlen(s);
	result = malloc(sizeof(char) * len + 1);
	if (!result)
		return (NULL);
	while (i < len && s[i] != '\n')
	{
		result[i] = s[i];
		i = i + 1;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_strjoin_shell(char *first, char *second)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(sizeof(char) * (ft_strlen(first) + ft_strlen(second) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (first[i] != '\0')
	{
		result[i] = first[i];
		i++;
	}
	j = 0;
	while (second[j] != '\0')
	{
		result[i + j] = second[j];
		j++;
	}
	result[i + j] = '\0';
	free(first);
	return (result);
}
