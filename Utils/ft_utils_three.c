/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_three.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 07:25:55 by aelison           #+#    #+#             */
/*   Updated: 2024/11/12 07:28:27 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static unsigned int	len_nb(long long nb)
{
	unsigned int	result;

	if (nb < 0)
	{
		result = 1;
		nb = nb * -1;
	}
	else
		result = 0;
	while (nb > 9)
	{
		result = result + 1;
		nb = nb / 10;
	}
	if (nb >= 0 && nb <= 9)
		result = result + 1;
	return (result);
}

static char	*negatif(char *result, long long nb, int len)
{
	result[0] = '-';
	nb = nb * -1;
	while (len)
	{
		result[len] = (nb % 10) + '0';
		nb = nb / 10;
		len = len - 1;
	}
	return (result);
}

static char	*positif(char *result, long long nb, int len)
{
	while (len)
	{
		result[len] = (nb % 10) + '0';
		nb = nb / 10;
		len = len - 1;
	}
	result[0] = nb + '0';
	return (result);
}

char	*ft_itoa_shell(long long nb)
{
	unsigned int	len;
	char			*result;

	len = len_nb(nb);
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	if (nb < 0)
		result = negatif(result, nb, len - 1);
	else
		result = positif(result, nb, len - 1);
	return (result);
}
