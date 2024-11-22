/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelison <aelison@student.42antananarivo.m  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:07:34 by aelison           #+#    #+#             */
/*   Updated: 2024/11/22 12:19:12 by aelison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_strlen_gnl(char *str);
int		ft_end_of_line(char *str);
char	*get_next_line(int fd);
char	*ft_strdup_gnl(char *str);
char	*ft_substr_gnl(char *str, int end);
char	*ft_strjoin_gnl(char *first, char *second);

#endif
