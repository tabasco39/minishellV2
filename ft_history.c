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
		fd = open(all_var->path_history, O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
	else if (do_append == EXIT_FAILURE)
		fd = open(all_var->path_history, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (fd == -1)
		return (fd);
	line = "\0";
	while (line && fd != -1)
	{
		line = get_next_line(fd);
		if (line)
		{
			tmp = ft_strdup_shell(line);
			add_history(tmp);
			free(line);
			free(tmp);
			all_var->nb_command++;
		}
	}
	return (fd);
}

void	ft_history_exist(t_var *all_var)
{
	int	new_fd;

	new_fd = open(all_var->path_history, O_RDONLY);
	if (new_fd == -1)
	{
		if (all_var->history != -1)
			close(all_var->history);
		rl_clear_history();
		all_var->nb_command = 0;
		all_var->history = ft_minishell_history(all_var, EXIT_SUCCESS);
	}
	if (new_fd != -1)
		close(new_fd);
}

void	ft_history(t_var *all_var)
{
	if (all_var->history == -1)
		return ;
	ft_history_exist(all_var);
	if (all_var->line[0] != '\0' && all_var->history != -1)
	{
		add_history(all_var->line);
		ft_putendl_fd(all_var->line, all_var->history);
		all_var->nb_command++;
	}
	if (all_var->nb_command > MAX_CMD)
	{
		if (all_var->history != -1)
			close(all_var->history);
		rl_clear_history();
		all_var->nb_command = 0;
		all_var->history = ft_minishell_history(all_var, EXIT_FAILURE);
		if (all_var->line[0] != '\0' && all_var->history != -1)
			ft_putendl_fd(all_var->line, all_var->history);
	}
}
