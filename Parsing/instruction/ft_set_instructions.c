/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_instructions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 09:13:39 by aelison           #+#    #+#             */
/*   Updated: 2024/11/15 08:09:41 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_count_token_in_instru(t_instru *current)
{
	int		result;
	t_token	*head;

	result = 1;
	if (!current || !current->start || !current->end)
		return (0);
	head = current->start;
	while (head && head != current->end)
	{
		result++;
		head = head->next;
	}
	return (result);
}

static t_instru	*ft_create_instru(t_token *head, t_token *end, int id)
{
	t_instru	*result;

	result = malloc(sizeof(t_instru));
	if (!result)
		return (NULL);
	result->id = id;
	result->start = head;
	result->end = end;
	result->next = NULL;
	result->prev = NULL;
	return (result);
}

static void	ft_add_instru(t_instru **head, t_instru *elem)
{
	t_instru	*tmp;

	if (*head == NULL)
		*head = elem;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = elem;
		elem->prev = tmp;
	}
}

void	ft_set_instru(t_instru **test, t_token *head)
{
	t_token	*start;
	t_token	*end;
	int		id;

	id = 1;
	start = NULL;
	end = NULL;
	while (head)
	{
		if (head->is_head == 1)
			start = head;
		if (head->is_end == 1)
			end = head;
		if (start && end)
		{
			ft_add_instru(test, ft_create_instru(start, end, id));
			id++;
			start = NULL;
			end = NULL;
		}
		head = head->next;
	}
}

static void	ft_reapply(t_var *var, int is_arg, char *exp)
{
	int		i;
	char	**tmp;
	char	*stack;

	i = 0;
	var = (void *)var;
	if (ft_find_char(exp, ' ') != -1)
		is_arg = EXIT_SUCCESS;
	tmp = ft_split_shell(exp, ' ');
	while (tmp && tmp[i])
	{
		stack  = ft_strtrim(tmp[i], " ");
		ft_add_token(&var->token, ft_create_token(stack), is_arg);
		free(stack);
		i++;
	}
	ft_free_all(tmp);
}

void	ft_apply(t_var *var, char *to_tkn, int is_arg, int is_value)
{
	t_list	*stack;
	t_list	*test;
	char	*list;
	char	*exp;
	char	*hehe;

	test = ft_divide_all(to_tkn);
	stack = test;
	hehe = ft_strdup("\0");
	while (test)
	{
		list = ft_list_to_not_expand(test->content);
		ft_define_exp_del_quote(&exp, list, test->content);
		if (ft_first_quote(test->content, '\'', '\"') == '\0' && is_value == EXIT_FAILURE)
		{
			if (ft_find_char(exp, ' ') != -1)
				ft_reapply(var, is_arg, exp);
			else
				hehe = ft_strjoin_shell(hehe, exp);
		}
		else
			hehe = ft_strjoin_shell(hehe, exp);	
		free(exp);
		free(list);
		test = test->next;
	}
	ft_add_token(&var->token, ft_create_token(hehe), is_arg);
	free(hehe);
	ft_lstclear(&stack, free);
}
