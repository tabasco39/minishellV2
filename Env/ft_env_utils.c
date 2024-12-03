/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 08:19:16 by aelison           #+#    #+#             */
/*   Updated: 2024/11/29 07:48:16 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_list	*ft_move_to_target(t_list *env, char *target)
{
	size_t	len;

	if (!env || !target)
		return (NULL);
	len = ft_strlen(target);
	while (env && ft_strncmp(env->content, target, len) != 0)
		env = env->next;
	return (env);
}

static void	ft_add_aux(t_var *var, char *to_add)
{
	ft_lstadd_back(&var->env, ft_lstnew(ft_strdup(to_add)));
	ft_free_all(var->tab_env);
	var->tab_env = ft_new_envp(var->env);
}

int	ft_unset_utils(t_var *var, t_list **tmp, t_list **after, char *to_del)
{
	char	*content_one;
	char	*content_two;
	size_t	len;

	content_one = (*tmp)->content;
	content_two = (*after)->content;
	len = ft_strlen(to_del);
	if (ft_strncmp(content_two, to_del, len) == 0)
	{
		if (content_two[len] == '\0' || content_two[len] == '=')
		{
			(*tmp)->next = (*after)->next;
			ft_lstdelone(*after, free);
			ft_free_all(var->tab_env);
			var->tab_env = ft_new_envp(var->env);
			return (0);
		}
	}
	return (1);
}

void	ft_add_to_env(t_var *var, char *to_add, char *val_env, int ind_equal)
{
	char	*exist;
	t_list	*tmp;

	exist = ft_getvar(var->env, val_env);
	if (exist == NULL)
		ft_add_aux(var, to_add);
	else
	{
		if (ind_equal != -1)
		{
			tmp = ft_move_to_target(var->env, val_env);
			free(tmp->content);
			tmp->content = ft_strjoin(val_env, to_add + ind_equal);
			ft_free_all(var->tab_env);
			var->tab_env = ft_new_envp(var->env);
		}
	}
}

void	ft_upgrade_env(t_var *var, char *var_name, char *new_val)
{
	t_list	*var_char;

	var_char = ft_move_to_target(var->env, var_name);
	if (var_char && new_val)
	{
		free(var_char->content);
		var_char->content = ft_strjoin(var_name, "=");
		var_char->content = ft_strjoin_shell(var_char->content, new_val);
		ft_free_all(var->tab_env);
		var->tab_env = ft_new_envp(var->env);
	}
}
