/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 06:39:42 by aelison           #+#    #+#             */
/*   Updated: 2024/12/02 07:13:42 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static unsigned int	count_c(char *p1, char c)
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
		ft_utils_count_c(p1, &i);
		if (p1[i] == c && p1[i + 1] != c && p1[i + 1] != '\0')
			count = count + 1;
		i++;
	}
	return (count);
}

static void	split_aux(char **result, char *s, char c)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (i < count_c(s, c))
	{
		while (s[j] == c && s[j] != '\0')
			j = j + 1;
		if (s[j] != c)
		{
			result[i] = ft_utils_split_aux(&j, s);
			if (!result[i])
			{
				free_all(result, count_c(s, c));
				i = count_c(s, c);
			}
		}
		i = i + 1;
	}
	result[i] = NULL;
}

char	**ft_split_shell(char *s, char c)
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

void	ft_update_has_redirection(t_token *target, int *has_redirection)
{
	if (target->command == redirect_output
		|| target->command == append_redirect_output)
		*has_redirection = 1;
}
