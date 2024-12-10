/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 10:50:37 by aelison           #+#    #+#             */
/*   Updated: 2024/12/10 10:50:49 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_reapply(t_var *var, int is_arg, char *exp)
{
	int		i;
	char	**tmp;
	char	*stack;

	i = 0;
	var = (void *)var;
	if (ft_find_char(exp, ' ') != -1)
		is_arg = EXIT_SUCCESS;
	tmp = ft_split_shell(exp, ' ');
	while (tmp && tmp[i])
	{
		stack = ft_strtrim(tmp[i], " ");
		ft_add_token(&var->token, ft_create_token(stack), is_arg);
		free(stack);
		i++;
	}
	ft_free_all(tmp);
}

static char	*ft_apply_aux(t_var *var, t_list *divide, int is_arg, int is_value)
{
	char	*result;
	char	*not_exp;
	char	*exp;

	result = ft_strdup("\0");
	while (divide)
	{
		not_exp = ft_list_to_not_expand(divide->content);
		ft_define_exp_del_quote(&exp, not_exp, divide->content);
		if (ft_first_quote(divide->content, '\'', '\"') == '\0'
			&& is_value == EXIT_FAILURE)
		{
			if (ft_find_char(exp, ' ') != -1)
				ft_reapply(var, is_arg, exp);
			else
				result = ft_strjoin_shell(result, exp);
		}
		else
			result = ft_strjoin_shell(result, exp);
		free(exp);
		free(not_exp);
		divide = divide->next;
	}
	return (result);
}

void	ft_apply(t_var *var, char *to_tkn, int is_arg, int is_value)
{
	t_list	*list;
	char	*token_word;

	list = ft_divide_all(to_tkn);
	token_word = ft_apply_aux(var, list, is_arg, is_value);
	ft_add_token(&var->token, ft_create_token(token_word), is_arg);
	free(token_word);
	ft_lstclear(&list, free);
}
