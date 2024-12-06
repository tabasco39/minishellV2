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
		tmp = ft_define_quote(split);
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
	int		i;
	int		is_arg;
	int		is_value;
	char	*exp;

	i = 0;
	is_arg = EXIT_FAILURE;
	is_value = EXIT_FAILURE;
	if (ft_find_char(to_tkn, '=') != -1 && ft_find_char(to_tkn, '$') != -1)
		is_value = EXIT_SUCCESS;
	exp = ft_expand(var, ft_strdup(to_tkn), EXIT_FAILURE);
	if (q_ref != '\0')
	{
		if (ft_find_char(to_tkn, '<') != -1
			|| ft_find_char(to_tkn, '>') != -1
			|| ft_find_char(to_tkn, '|') != -1)
			is_arg = EXIT_SUCCESS;
	}
	if (q_ref == '\0' && is_value == EXIT_FAILURE)
		ft_reapply(var, q_ref, is_arg, exp);
	else
		ft_div_by_token_aux(var, exp, q_ref, is_arg);
	free(exp);
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
