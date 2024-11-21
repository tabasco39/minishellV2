/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_program.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:38:32 by aelison           #+#    #+#             */
/*   Updated: 2024/11/21 10:15:44 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_simple_interrupt(int signal, siginfo_t *proc, void *i)
{
	t_var	*all;

	all = ft_get_struct_var();
	if (signal == SIGINT && proc->si_pid != 0)
	{
		all->status = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("\0", 0);
		rl_redisplay();
	}
	i++;
}

void	ft_handle_signal(void)
{
	struct sigaction	act;

	act.sa_sigaction = ft_simple_interrupt;
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
}

t_var	*ft_get_struct_var(void)
{
	static t_var	all;

	return (&all);
}

void	ft_init_var(t_var *all, char **envp)
{
	all->status = 0;
	all->token = NULL;
	all->env = NULL;
	all->line = NULL;
	all->instru = NULL;
	all->hdoc_line = NULL;
	all->current_status = 0;
	ft_create_envp(&all->env, envp);
	all->tab_env = ft_new_envp(all->env);
	all->iteration = malloc(sizeof(t_exec));
	all->iteration->redir_in_fd = STDIN_FILENO;
	all->iteration->pipefd[0] = -1;
	all->iteration->pipefd[1] = -1;
}

int	main(int argc, char **argv, char **envp)
{
	t_var	*all_var;

	all_var = ft_get_struct_var();
	if (argc != 1 && argv && envp)
	{
		ft_putstr_fd("error : no arg need\n", 2);
		return (EXIT_FAILURE);
	}
	ft_init_var(all_var, envp);
	while (1)
	{
		ft_handle_signal();
		all_var->line = readline(GREEN"Minishell$> "RESET);
		if (all_var->line == NULL)
			ft_exit(all_var, all_var->status, 0);
		add_history(all_var->line);
		ft_debug(all_var);
		close(3);
		close(4);
	}
	return (all_var->status);
}
