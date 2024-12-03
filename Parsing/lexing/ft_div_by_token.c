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

void	ft_div_by_token_aux(char *before, char *split, t_token **head)
{
	char	*tmp;

	if (before && (ft_strncmp(before, "<", 1) == 0
			|| ft_strncmp(before, ">", 1) == 0))
	{
		ft_add_token(head, ft_create_token(split));
		return ;
	}
	tmp = ft_define_quote(split);
	ft_add_token(head, ft_create_token(tmp));
	free(tmp);
}

int	ft_div_by_token(t_var *var)
{
	char	**split;
	char	*newline;
	int		i;

	i = 0;
	newline = ft_div_by_redirect(var->line, "<>|");
	newline = ft_expand(var, newline, EXIT_FAILURE);
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
			ft_div_by_token_aux(NULL, split[i], &var->token);
		else
			ft_div_by_token_aux(split[i - 1], split[i], &var->token);
		i++;
	}
	ft_free_all(split);
	return (EXIT_SUCCESS);
}
