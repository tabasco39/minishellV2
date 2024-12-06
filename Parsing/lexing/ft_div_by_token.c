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

static void	ft_reapply(t_var *var, int is_arg, char *exp)
{
	int		i;
	char	**tmp;

	i = 0;
	if (ft_find_char(exp, ' ') != -1)
		is_arg = EXIT_SUCCESS;
	tmp = ft_split_shell(exp, ' ');
	while (tmp && tmp[i])
	{
		ft_add_token(&var->token, ft_create_token(tmp[i]), is_arg);
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

	is_value = EXIT_FAILURE;
	if (ft_find_char(to_tkn, '=') != -1 && ft_find_char(to_tkn, '$') != -1)
		is_value = EXIT_SUCCESS;
	list = ft_list_to_not_expand(to_tkn);
	ft_define_exp_del_quote(&exp, &tmp, list, to_tkn);
	is_arg = ft_tkn_errors(to_tkn, exp, tmp, q_ref);
	if (q_ref == '\0' && is_value == EXIT_FAILURE)
		ft_reapply(var, is_arg, exp);
	else
		ft_add_token(&var->token, ft_create_token(exp), is_arg);
	free(list);
	free(exp);
	free(tmp);
}

static char	*ft_loop_dollar(char *to_change)
{
	int		i;
	char	tmp[2];
	char	*result;

	i = 0;
	tmp[1] = '\0';
	result = ft_strdup("\0");
	while (to_change[i] != '\0')
	{
		tmp[0] = to_change[i];
		if (tmp[0] == '\'' || tmp[0] == '\"')
			ft_join_until_close_quote(&result, to_change, &i, tmp);
		result = ft_strjoin_shell(result, tmp);
		if (ft_add_special(tmp, to_change, &i, &result) == EXIT_SUCCESS)
			continue ;
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
	char	*tmp;

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
		tmp = ft_loop_dollar(to_tokenize);
		ft_tkn_aux(var, quote_ref, tmp);
		free(tmp);
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
