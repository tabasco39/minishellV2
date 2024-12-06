/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:44:19 by aelison           #+#    #+#             */
/*   Updated: 2024/12/06 13:44:29 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_list_aux(char *check, int *i, char tmp, int *flag)
{
	char	*result;
	char	*value;
	int		exp;

	exp = 0;
	result = ft_strdup("\0");
	if (ft_is_open(check, (*i) + 1) == EXIT_SUCCESS)
	{
		(*i)++;
		if (tmp == '\'' || check[*i + 1] == tmp)
			exp = 1;
		while (check[*i] && check[*i] != tmp)
		{
			if (check[*i] == '$')
				(*flag)++;
			if (check[*i] == '$' && exp == 1)
			{
				value = ft_itoa(*flag);
				result = ft_strjoin_shell(result, value);
				free(value);
			}
			(*i)++;
		}
	}
	return (result);
}

char	*ft_div_expand_aux(char *change, int *i, char tmp[2])
{
	char	*result;
	char	stack[2];
	char	div[2];

	result = ft_strdup("\0");
	stack[1] = '\0';
	div[0] = ' ';
	div[1] = '\0';
	if (*i != 0 && (change[*i - 1] == '_' || ft_isalnum(change[*i - 1]) != 0))
		result = ft_strjoin_shell(result, div);
	result = ft_strjoin_shell(result, tmp);
	(*i)++;
	while (change[*i] && change[*i] != '$')
	{
		stack[0] = change[*i];
		result = ft_strjoin_shell(result, stack);
		(*i)++;
	}
	if (change[*i] == '$')
		(*i)--;
	return (result);
}

int	ft_del_dollar(char *to_change, int ind_dollar)
{
	if (ind_dollar != -1 && to_change[ind_dollar + 1])
	{
		ind_dollar++;
		if (to_change[ind_dollar] == '\'' || to_change[ind_dollar] == '\"')
		{
			if (ft_is_open(to_change, ind_dollar + 1) == EXIT_SUCCESS)
				return (EXIT_SUCCESS);
		}
	}
	return (EXIT_FAILURE);
}

int	ft_tkn_errors(char *to_tkn, char *exp, char *tmp, char q_ref)
{
	int		is_arg;

	is_arg = EXIT_FAILURE;
	if (ft_find_char(exp, '<') != -1 || ft_find_char(exp, '>') != -1)
	{
		if (ft_find_char(to_tkn, '<') == -1 && ft_find_char(to_tkn, '>') == -1)
			is_arg = EXIT_SUCCESS;
	}
	if (q_ref != '\0')
	{
		if (ft_find_char(tmp, '<') != -1
			|| ft_find_char(tmp, '>') != -1
			|| ft_find_char(tmp, '|') != -1)
			is_arg = EXIT_SUCCESS;
	}
	return (is_arg);
}

void	ft_define_exp_del_quote(char **exp, char **tmp,
		char *list, char *to_tkn)
{
	t_var	*var;

	var = ft_get_struct_var();
	*exp = NULL;
	*tmp = NULL;
	if (ft_find_char(to_tkn, ' ') != -1)
	{
		*exp = ft_expand(var, ft_strdup(to_tkn), EXIT_FAILURE);
		*tmp = ft_define_quote(*exp);
		free(*exp);
		*exp = ft_strdup(*tmp);
	}
	else
	{
		*tmp = ft_define_quote(to_tkn);
		*exp = ft_expand_parse(var, *tmp, list);
	}
}
