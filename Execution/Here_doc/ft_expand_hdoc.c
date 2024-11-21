/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_hdoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 07:48:30 by aelison           #+#    #+#             */
/*   Updated: 2024/11/21 07:53:17 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_loop(char *to_change)
{
	int	i;

	i = 0;
	while (to_change[i] == '$')
		i++;
	if (to_change[i] != 39 && to_change[i] != 34)
		return (ft_strdup(to_change));
	if (i % 2 == 0)
		return (ft_strdup(to_change));
	else
	{
		if (i != 1)
			return (ft_strdup(to_change + 1));
		else
			return (ft_strdup(to_change));
	}
}

char	*ft_expand_hdoc(char *to_change, int *bool)
{
	char	*result;
	char	*tmp;
	int		i;

	i = 0;
	result = ft_loop(to_change);
	if (ft_find_char(to_change, 34) != -1)
		*bool = 1;
	if (ft_find_char(to_change, 39) != -1)
		*bool = 1;
	tmp = ft_del_quote(result, "\"\'");
	free(result);
	result = ft_strdup(tmp);
	free(tmp);
	free(to_change);
	return (result);
}

void	ft_interupt_and_exit(int signal)
{
	t_var	*all;

	all = ft_get_struct_var();
	if (signal == SIGINT)
	{
		ft_free_minishell(all);
		rl_on_new_line();
		ft_putchar_fd('\n', STDOUT_FILENO);
		exit (130);
	}
}
