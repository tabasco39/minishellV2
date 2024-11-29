/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_pid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:27:10 by aelison           #+#    #+#             */
/*   Updated: 2024/11/29 10:17:53 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_check_pid(char *path)
{
	void		*ptr;
	char		*tmp;
	size_t		i;

	i = ft_strlen(path);
	while (--i)
	{
		if (path[i] == '/')
			break ;
	}
	tmp = ft_substr(path, 0, i);
	ptr = opendir(tmp);
	free(tmp);
	if (ptr == NULL)
		return (EXIT_FAILURE);
	closedir(ptr);
	if (access(path, F_OK) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	*ft_get_pid(void)
{
	int		fd;
	char	*line;
	char	*result;

	result = NULL;
	line = "";
	if (ft_check_pid("/proc/self/status") == EXIT_FAILURE)
		return (NULL);
	fd = open("/proc/self/status", O_RDONLY);
	if (fd == -1)
		return (NULL);
	while (line)
	{
		line = get_next_line(fd);
		if (line)
		{
			if (ft_strncmp(line, "Pid", 3) == 0)
				result = ft_strtrim(line + 4, " \n\t");
			free(line);
		}
	}
	close(fd);
	return (result);
}

char	*ft_exp(t_var *var, char *arg, int start, char *status)
{
	char	*result;

	result = NULL;
	if (arg[start] == '$')
		result = ft_get_pid();
	else if (arg[start] == '?')
		result = ft_strdup(status);
	else if (arg[start] == '0')
		result = ft_strdup("minishell");
	else
		result = ft_expand_res(var->env, arg, start, 1);
	return (result);
}

static int	ft_exp_aux(char *to_check, int do_exp, char *ref, int *i)
{
	if (do_exp == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	*ref = ft_first_quote(to_check, 34, 39);
	if (*ref == '\0')
		return (EXIT_SUCCESS);
	*ref = '\0';
	*i = 0;
	return (EXIT_FAILURE);
}

int	ft_exec_exp(char *to_check, int end, int do_exp)
{
	int		i;
	char	q_ref;

	if (ft_exp_aux(to_check, do_exp, &q_ref, &i) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	while (i != end && to_check[i])
	{
		if (to_check[i] == (char)34 || to_check[i] == (char)39)
		{
			if (q_ref == '\0')
				q_ref = to_check[i];
			else
			{
				if (q_ref == to_check[i])
					q_ref = '\0';
			}
		}
		i++;
	}
	if (q_ref == '\0' || q_ref == (char)34)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
