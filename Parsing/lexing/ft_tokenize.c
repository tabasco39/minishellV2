/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:47:54 by aelison           #+#    #+#             */
/*   Updated: 2024/12/05 10:48:04 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char    *ft_list_to_not_expand(char *check)
{
    char    tmp;
	char	*value;
    char    *result;
    int     flag;
    int     i;

    i = 0;
    flag = -1;
    result = ft_strdup("\0");
    if (!check)
        return (result);
    while (check[i] != '\0')
    {
        tmp = check[i];
        if (tmp == '$')
            flag++;
        if (tmp == '\'' || tmp == '\"')
        {
            if (ft_is_open(check, i + 1) == EXIT_SUCCESS)
            {
                i++;
                while (check[i] && check[i] != tmp)
                {
                    if (check[i] == '$' && tmp == '\'')
                    {
                    	flag++;
						value = ft_itoa(flag);
                        result = ft_strjoin_shell(result, value);
						free(value);
                    }
                    i++;
                }
            }
        }
        if (check[i] != '\0')
            i++;
    }
    return (result);
}

char	*ft_div_expand(char *to_change)
{
	char	*result;
	char	tmp[2];
	char	stop[2];
	char	div[2];
	int		i;

	i = 0;
	tmp[1] = '\0';
	div[0] = ' ';
	div[1] = '\0';
	stop[1] = '\0';
	result = ft_strdup("\0");
	if (!to_change)
		return (result);
	while (to_change[i] != '\0')
	{
		tmp[0] = to_change[i];
		if (tmp[0] == '$')
		{
			if (i != 0 && (to_change[i - 1] == '_' || ft_isalnum(to_change[i - 1]) != 0))
				result = ft_strjoin_shell(result, div);
			result = ft_strjoin_shell(result, tmp);
			i++;
			while (to_change[i] && to_change[i] != '$')
			{
				stop[0] = to_change[i];
				result = ft_strjoin_shell(result, stop);
				i++;
			}
			if (to_change[i] == '$')
				i--;
		}
		else
			result = ft_strjoin_shell(result, tmp);
		if (to_change[i] != '\0')
			i++;
	}
	return (result);
}

char	*ft_expand_parse(t_var *var, char *to_exp, char *not_exp)
{
	char	*divide;
	char	**tmp;
	char	*result;
	int		i;

	i = 0;
	divide = ft_div_expand(to_exp);
	tmp = ft_split(divide, ' ');
	while (tmp[i])
	{
		if (ft_find_char(not_exp, i + '0') == -1)
			tmp[i] = ft_expand(var, tmp[i], EXIT_SUCCESS);
		i++;
	}
	i = 0;
	result = ft_strdup("\0");
	while (tmp[i])
	{
		result = ft_strjoin_shell(result, tmp[i]);
		i++;
	}
	i = 0;
	while (tmp[i])
	{
		free(tmp[i]);
		i++;
	}
	free(tmp);
	free(divide);
	return (result);
}
