/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_div_by_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 10:34:47 by aelison           #+#    #+#             */
/*   Updated: 2024/10/10 09:32:09 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_should_delete(char *line)
{
	int		i;

	i = -1;
	printf("line = %s\n", line);
	while (line[++i] != '\0')
	{
		if (line[i] == '$')
			return (0);
	}
	return (1);
}

char	*ft_clean_quote(char *line)
{
	int		i;
	char	simple_quote;
	char	double_quote;
	char	*result;

	i = 0;
	simple_quote = (char)39;
	double_quote = (char)34;
	result = NULL;
	while (line[i] != '\0')
	{
		if (line[i] == simple_quote && line[i + 1] == simple_quote)
		{
			result = ft_del_quote(line, &simple_quote);
			return (result);
		}
		else if (line[i] == double_quote && line[i + 1] == double_quote)
		{
			result = ft_del_quote(line, &double_quote);
			return (result);
		}
		i++;
	}
	return (line);
}

void	ft_div_by_token_aux(char *split, t_token **head)
{
	char	*tmp;
	char	*newline;

	newline = ft_get_first_quote(split);
	if (ft_should_delete(split) == 1)
	{
		tmp = ft_del_quote(split, newline);
		ft_add_token(head, ft_create_token(tmp));
		free(tmp);
	}
	else
		ft_add_token(head, ft_create_token(split));
	free(newline);
}
int		ft_do_expand(char *line)
{
	int	i;

	i = 0;
	if (ft_first_quote(line, (char)34, (char)39) == 39)
		return (EXIT_FAILURE);
	while (line[i] != '\0')
	{
		if (line[i] == '>')
		{
			while ((line[i] == '>' || line[i] == ' ') && line[i])
				i++;
			if (line[i] && line[i] == '$')
				return (EXIT_FAILURE);
		}
		if (line[i])
			i++;
	}
	return (EXIT_SUCCESS);
}

void	ft_div_by_token(t_var *var)
{
	char	**split;
	char	*newline;
	int		i;

	i = 0;
	newline = ft_div_by_redirect(var->line, "<>|");
	newline = ft_parse_final(var, newline);
	split = ft_split_shell(newline, ' ');
	if (split == NULL || *split == NULL)
		return ;
	free(newline);
	newline = NULL;
	while (split[i])
	{
		ft_div_by_token_aux(split[i], &var->token);
		free(split[i]);
		i++;
	}
	free(split);
}
