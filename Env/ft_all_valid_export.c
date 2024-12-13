/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_all_valid_export.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:01:52 by aelison           #+#    #+#             */
/*   Updated: 2024/12/12 16:02:07 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_all_valid_export(t_token *start)
{
    int     result;

    result = EXIT_SUCCESS;
    if (!start)
        return (EXIT_FAILURE);
    while (start)
    {
        if (start->token[0] == '\0')
            result = EXIT_FAILURE;
        else
            result = EXIT_SUCCESS;
        start = start->next;
    }
    return (result);
}