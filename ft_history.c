/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 13:03:52 by aelison           #+#    #+#             */
/*   Updated: 2024/11/22 14:36:22 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_minishell_history(t_var *all_var, int do_append)
{
	int		fd;
	char	*line;
	char	*tmp;

	fd = -1;
	if (do_append == EXIT_SUCCESS)
		fd = open(".minishell_history", O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
	else if (do_append == EXIT_FAILURE)
		fd = open(".minishell_history", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	line = "\0";
	while (line)
	{
		line = get_next_line(fd);
		if (line)
		{
			tmp = ft_strdup_shell(line);
			add_history(tmp);
			all_var->nb_command++;
			free(line);
			free(tmp);
		}
	}
	return (fd);
}

void	ft_history_exist(t_var *all_var)
{
	int	new_fd;

	new_fd = open(".minishell_history", O_RDONLY);
	if (new_fd == -1)
	{
		rl_clear_history();
		close(all_var->history);
		all_var->nb_command = 0;
		all_var->history = ft_minishell_history(all_var, EXIT_SUCCESS);
	}
	if (new_fd != -1)
		close(new_fd);
}

void	ft_history(t_var *all_var)
{
	ft_history_exist(all_var);
	add_history(all_var->line);
	all_var->nb_command++;
	if (all_var->line[0] != '\0')
		ft_putendl_fd(all_var->line, all_var->history);
	if (all_var->nb_command > MAX_CMD)
	{
		rl_clear_history();
		all_var->nb_command = 0;
		close(all_var->history);
		all_var->history = ft_minishell_history(all_var, EXIT_FAILURE);
		if (all_var->line[0] != '\0')
			ft_putendl_fd(all_var->line, all_var->history);
	}
}
