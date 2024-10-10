/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 08:57:29 by aelison           #+#    #+#             */
/*   Updated: 2024/10/10 07:19:25 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*		TA NORM LAAAAA */

char	*ft_parse_final(t_var *var, char *to_change)
{
	char	*res;
	char	*tmp;
	char	*stack;
	int		start;
	int		end;

	start = 0;
	end = 0;
	res = ft_strdup("\0");
	//printf("to change = %s\n", to_change);
	if (ft_do_expand(to_change) == EXIT_FAILURE)
	{
		//printf("do expand\n");
		return (ft_strdup(to_change));
	}
	while (to_change[end] != '\0')
	{
		if (to_change[end] == '$' && end != 0)
		{
			tmp = ft_substr(to_change, start, end - start);
			res = ft_strjoin_shell(res, tmp);
			free(tmp);
			start = end;
		}
		else if (to_change[start] == '$')
		{
			if (end == start)
				end++;
			if (to_change[end] != '\0' && ft_isalpha(to_change[end]) == 0 && to_change[end] != '_')
			{
				if (to_change[end] == '?')
				{
					stack = ft_strdup("007");
					res = ft_strjoin_shell(res, stack);
				}
				start = end + 1;
			}
			else
			{
				while (to_change[end] != '\0' && to_change[end] != ' ' && to_change[end] != '$' && to_change[end] != (char)34 && to_change[end] != (char)39 && to_change[end] != '\n')
					end++;
				stack = ft_substr(to_change, start, end - start);
				if (stack && stack[1] != '\0')
				{
					tmp = ft_getvar(var->env, stack + 1);
					tmp = tmp + ft_find_char(tmp, '=') + 1;
					if (tmp == NULL)
						tmp = "\0";
					res = ft_strjoin_shell(res, tmp);
				}
				else
					res = ft_strjoin_shell(res, stack);
				start = end;
			}
			free(stack);
		}
		end++;
	}
	stack = ft_substr(to_change, start, end - start);
	res = ft_strjoin_shell(res, stack);
	free(stack);
	free(to_change);
	return (res);
}
