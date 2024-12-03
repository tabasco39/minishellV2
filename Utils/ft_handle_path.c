/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 09:12:09 by aranaivo          #+#    #+#             */
/*   Updated: 2024/11/20 08:01:08 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_substitute_var(char *token)
{
	char	**result;

	result = malloc(sizeof(char *) * 2);
	if (!result)
		return (NULL);
	result[0] = ft_strjoin("\0", token);
	result[1] = NULL;
	if (access(result[0], X_OK) != 0)
	{
		ft_free_all(result);
		return (NULL);
	}
	return (result);
}

char	**ft_get_all_path(t_list *env, char *line)
{
	char	*paths;
	char	**paths_tab;
	int		i;

	i = 0;
	if (ft_find_char(line, '/') != -1)
	{
		paths_tab = malloc(sizeof(char *) * 2);
		paths_tab[0] = ft_strdup(line);
		paths_tab[1] = NULL;
		return (paths_tab);
	}
	paths = ft_getvar(env, "PATH");
	if (paths == NULL)
		return (ft_substitute_var(line));
	paths += 5;
	paths_tab = ft_split(paths, ':');
	while (line && paths_tab[i] != NULL)
	{
		paths_tab[i] = ft_strjoin_shell(paths_tab[i], "/");
		paths_tab[i] = ft_strjoin_shell(paths_tab[i], line);
		i++;
	}
	return (paths_tab);
}

char	*ft_verify_exec_cmd(char **paths)
{
	int		i;
	char	*result;

	i = 0;
	while (paths[i] != NULL)
	{
		if (access(paths[i], X_OK) == 0)
		{
			result = ft_strdup_shell(paths[i]);
			return (result);
		}
		i++;
	}
	return (NULL);
}
