/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_envp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 07:54:52 by aelison           #+#    #+#             */
/*   Updated: 2024/10/09 11:00:31 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_sort_aux(t_list **tmp, t_list **tmp_nxt, t_list **head)
{
	char	*val_one;
	char	*val_two;
	char	*stack;
	int		val;

	val_one = (char *)(*tmp)->content;
	val_two = (char *)(*tmp_nxt)->content;
	val = ft_strncmp(val_one, val_two, ft_strlen(val_one));
	if (val > 0)
	{
		stack = val_one;
		(*tmp)->content = (*tmp_nxt)->content;
		(*tmp_nxt)->content = stack;
		*tmp = *head;
		*tmp_nxt = (*tmp)->next;
	}
	else
	{
		*tmp = (*tmp)->next;
		*tmp_nxt = (*tmp)->next;
	}
}

void	ft_sort_env_list(t_list **result, char **tab_env)
{
	t_list	*tmp;
	t_list	*tmp_nxt;
	int		val;
	int		len;

	ft_create_envp(result, tab_env);
	tmp = *result;
	tmp_nxt = tmp->next;
	while (tmp_nxt)
	{
		len = ft_strlen(tmp->content);
		val = ft_strncmp(tmp->content, tmp_nxt->content, len);
		ft_sort_aux(&tmp, &tmp_nxt, result);
	}
}

int	ft_export(t_var *var, char *to_add)
{
	int		end;
	char	*var_envp;
	t_list	*tmp;

	tmp = NULL;
	if (to_add == NULL)
	{
		ft_sort_env_list(&tmp, var->tab_env);
		ft_display_env(&tmp, NULL);
		ft_lstclear(&tmp, free);
		return (EXIT_SUCCESS);
	}
	var_envp = NULL;
	end = ft_find_char(to_add, '=');
	if (end == -1)
		var_envp = ft_strdup(to_add);
	else
		var_envp = ft_substr(to_add, 0, end);
	ft_add_to_env(var, to_add, var_envp, end);
	free(var_envp);
	return (EXIT_SUCCESS);
}

int	ft_export_aux(t_var *var, char *to_add)
{
	char	**res_split;
	int		result;
	int		i;

	i = 0;
	if (ft_find_char(to_add, ' ') != -1 && ft_find_char(to_add, '=') == -1)
	{
		res_split = ft_split(to_add, ' ');
		while (res_split[i])
		{
			result = ft_export(var, res_split[i]);
			i++;
		}
		ft_free_all(res_split);
	}
	else
		result = ft_export(var, to_add);
	return (result);
}

char	*ft_env_variable(t_list *envp, char *to_find)
{
	int		end;
	char	*tmp;
	char	*result;
	char	*new_find;

	end = ft_find_char(to_find, ' ');
	new_find = ft_strdup(to_find);
	if (end != -1)
	{
		free(new_find);
		new_find = ft_substr(to_find, 0, end);
	}
	tmp = ft_getvar(envp, new_find);
	if (new_find)
		free(new_find);
	if (!tmp)
		return (NULL);
	result = ft_strchr(tmp, '=');
	if (!result)
		return (NULL);
	return (result + 1);
}
