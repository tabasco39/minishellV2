/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 06:59:12 by aelison           #+#    #+#             */
/*   Updated: 2024/10/10 08:31:27 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(char *to_print, char option)
{
	int		i;
	int		quote;
	int		count_quote;

	i = 0;
	count_quote = 0;
	quote = ft_first_quote(to_print, 34, 39);
	while (to_print[i] != '\0')
	{
		if (to_print[i] != quote)
		{
			if (to_print[i] == '$')
			{
				if (to_print[i + 1] == '\0')
					ft_putchar_fd(to_print[i], 1);
				if (count_quote % 2 != 0)
					ft_putchar_fd(to_print[i], 1);
			}
			else if (to_print[i] != '$')
				ft_putchar_fd(to_print[i], 1);
		}
		else
			count_quote++;
		i++;
	}
	if (option == '\n')
		ft_putchar_fd('\n', 1);
}

void	ft_oldpwd(t_var *var, char *current, char *to_change)
{
	char		*oldpwd;
	char		*tmp;
	int			len;

	len = ft_max_value(ft_strlen(current), ft_strlen(to_change));
	tmp = ft_getvar(var->env, "OLDPWD");
	oldpwd =  tmp + ft_find_char(tmp, '=') + 1;
	if (ft_strncmp(current, to_change,  len) != 0)
	{
		if (ft_strncmp(to_change, oldpwd, len) != 0)
			ft_upgrade_env(var, "OLDPWD", to_change);
	}
}

char	*ft_pwd(void)
{
	char	directory[1024];
	char	*result;

	result = getcwd(directory, 1024);
	if (result == NULL)
	{
		ft_putstr_fd("error : getcwd\n", 2);
		return (NULL);
	}
	ft_putendl_fd(result, 1);
	return (result);
}

int	ft_cd(char *path, t_var *var, char *var_name)
{
	char	 buff[1024];
	char	to_change[1024];
	char	*result;

	result = getcwd(to_change, 1024);
	if (chdir(path) == -1)
	{
		ft_putendl_fd("minishell: cd: No such file directory", 2);
		return (EXIT_FAILURE);
	}
	ft_upgrade_env(var, var_name, getcwd(buff, 1024));
	ft_oldpwd(var, getcwd(buff, 1024), result);
	return (EXIT_SUCCESS);
}
