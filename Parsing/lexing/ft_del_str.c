/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_del_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 08:23:56 by aelison           #+#    #+#             */
/*   Updated: 2024/11/15 08:33:24 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_rm_case(int q_case)
{
	char	*result;

	result = malloc(sizeof(char) * 3);
	if (!result)
		return (NULL);
	if (q_case == 0)
	{
		result[0] = (char)34;
		result[1] = (char)39;
		result[2] = '\0';
	}
	else if (q_case == 1)
	{
		result[0] = (char)34;
		result[1] = '\0';
		return (result);
	}
	return (result);
}

char	*ft_del_quote(char *word, char *quote)
{
	int		i;
	int		j;
	int		len;
	int		nb_quote;
	char	*result;

	nb_quote = ft_count_quote(word, quote);
	len = ft_strlen(word) - nb_quote + 1;
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (word[i] != '\0')
	{
		while (ft_find_char(quote, word[i]) != -1 && word[i] != '\0')
			i++;
		result[j] = word[i];
		j++;
		if (word[i] != '\0')
			i++;
	}
	result[j] = '\0';
	return (result);
}
