/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_edit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 05:24:56 by aelison           #+#    #+#             */
/*   Updated: 2024/11/29 08:53:20 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_div_aux(char *result, char div, int *j, int nb)
{
	result[*j] = ' ';
	*j = *j + 1;
	while (nb)
	{
		result[*j] = div;
		*j = *j + 1;
		nb--;
	}
	result[*j] = ' ';
}

void	ft_edit(char *line, int *i, char *result, int *j)
{
	if (line[*i + 1] && line[*i + 1] == '<' && line[*i] == '<' )
	{
		ft_div_aux(result, line[*i], j, 2);
		(*i)++;
	}
	else if (line[*i + 1] && line[*i + 1] == '>' && line[*i] == '>')
	{
		ft_div_aux(result, line[*i], j, 2);
		(*i)++;
	}
	else
		ft_div_aux(result, line[*i], j, 1);
}

int	ft_check_open_quote(char *line, int i)
{
	if (ft_find_char(line, 34) == -1 && ft_find_char(line, 39) == -1)
		return (EXIT_SUCCESS);
	if (ft_is_open(line, i) == EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (ft_is_open(line, i) == EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	*ft_div_by_redirect(char *line, char *ref)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(line) + ft_count_quote(line, ref) * 2;
	result = malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (ft_check_open_quote(line, i) == EXIT_SUCCESS
			&& ft_find_char(ref, line[i]) != -1)
			ft_edit(line, &i, result, &j);
		else
			result[j] = line[i];
		j++;
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	ft_add_special(char curr[2], char *to_change, int *i, char **result)
{
	if (curr[0] == '$' && ft_del_dollar(to_change, *i) == EXIT_SUCCESS)
	{
		*result = ft_strjoin_shell(*result, "$@");
		(*i)++;
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
