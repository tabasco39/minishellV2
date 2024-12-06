/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_div_by_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 10:34:47 by aelison           #+#    #+#             */
/*   Updated: 2024/11/25 10:16:14 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_div_by_token_aux(t_var *var, char *split, char q_ref, int is_arg)
{
	char	*tmp;

	tmp = NULL;
	if (q_ref != '\0')
		tmp = ft_strdup(split);
		//tmp = ft_define_quote(split);
	else
		tmp = ft_strdup(split);
	ft_add_token(&var->token, ft_create_token(tmp), is_arg);
	free(tmp);
}

static void	ft_reapply(t_var *var, char q_ref, int is_arg, char *exp)
{
	int		i;
	char	**tmp;

	i = 0;
	if (ft_find_char(exp, ' ') != -1)
		is_arg = EXIT_SUCCESS;
	tmp = ft_split_shell(exp, ' ');
	while (tmp && tmp[i])
	{
		ft_div_by_token_aux(var, tmp[i], q_ref, is_arg);
		i++;
	}
	ft_free_all(tmp);
}

static void	ft_tkn_aux(t_var *var, char q_ref, char *to_tkn)
{
	int		is_arg;
	int		is_value;
	char	*exp;
	char	*list;
	char	*tmp;

	is_arg = EXIT_FAILURE;
	is_value = EXIT_FAILURE;
	if (ft_find_char(to_tkn, '=') != -1 && ft_find_char(to_tkn, '$') != -1)
		is_value = EXIT_SUCCESS;
	list = ft_list_to_not_expand(to_tkn);
	exp = NULL;
	tmp = NULL;
	if (ft_find_char(to_tkn, ' ') != -1)
	{
		exp = ft_expand(var, ft_strdup(to_tkn), EXIT_FAILURE);
		tmp = ft_define_quote(exp);
		free(exp);
		exp = ft_strdup(tmp);
	}
	else
	{
		tmp = ft_define_quote(to_tkn);
		exp = ft_expand_parse(var, tmp, list);
	}
	if (q_ref != '\0')
	{
		if (ft_find_char(tmp, '<') != -1
			|| ft_find_char(tmp, '>') != -1
			|| ft_find_char(tmp, '|') != -1)
			is_arg = EXIT_SUCCESS;
	}
	if (q_ref == '\0' && is_value == EXIT_FAILURE)
		ft_reapply(var, q_ref, is_arg, exp);
	else
		ft_div_by_token_aux(var, exp, q_ref, is_arg);
	free(exp);
	free(tmp);
}

static int	ft_del_dollar(char *to_change, int ind_dollar)
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

static char	*ft_loop_dollar(char *to_change)
{
	int	i;
	char	test[2];
	char	test_1[2];
	char	*result;

	i = 0;
	test[1] = '\0';
	test_1[1] = '\0';
	result = ft_strdup("\0");
	while (to_change[i] != '\0')
	{
		test_1[0] = to_change[i];
		if (test_1[0] == '\'' || test_1[0] == '\"')
		{
			if (ft_is_open(to_change, i + 1) == EXIT_SUCCESS)
			{
				i++;
				result = ft_strjoin_shell(result, test_1);
				while (to_change[i] && to_change[i] != test_1[0])
				{
					test[0] = to_change[i];
					result = ft_strjoin_shell(result, test);
					i++;
				}
			}
		}
		if (test_1[0] == '$' && ft_del_dollar(to_change, i) == EXIT_SUCCESS)
		{
			result = ft_strjoin_shell(result, "$@");
			i++;
			continue ;
		}
		result = ft_strjoin_shell(result, test_1);
		if (to_change[i] != '\0')
			i++;
	}
	if (result[0] == '\0')
	{
		free(result);
		result = ft_strdup(to_change);
	}
	return (result);
}

static void	ft_tokenize(t_var *var, char *to_tokenize, char *before)
{
	char	quote_ref;

	if (to_tokenize)
	{
		quote_ref = ft_first_quote(to_tokenize, '\'', '\"');
		if (before)
		{
			if (ft_strncmp(before, "<", 1) == 0
				|| ft_strncmp(before, ">", 1) == 0)
			{
				ft_add_token(&var->token,
					ft_create_token(to_tokenize), EXIT_FAILURE);
				return ;
			}
		}
		to_tokenize = ft_loop_dollar(to_tokenize);
		ft_tkn_aux(var, quote_ref, to_tokenize);
	}
}

int	ft_div_by_token(t_var *var)
{
	char	**split;
	char	*newline;
	int		i;

	i = 0;
	newline = ft_div_by_redirect(var->line, "<>|");
	split = ft_split_shell(newline, ' ');
	if (split == NULL || *split == NULL)
	{
		free(newline);
		free(split);
		return (EXIT_FAILURE);
	}
	free(newline);
	while (split[i])
	{
		if (i == 0)
			ft_tokenize(var, split[i], NULL);
		else
			ft_tokenize(var, split[i], split[i - 1]);
		i++;
	}
	ft_free_all(split);
	return (EXIT_SUCCESS);
}
