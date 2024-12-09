/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranaivo <aranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 10:02:09 by aelison           #+#    #+#             */
/*   Updated: 2024/12/06 15:29:01 by aranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/get_next_line/get_next_line.h"
# include "./libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <unistd.h>

# define RED "\001\e[0;31m\002"
# define GREEN "\001\e[0;32m\002"
# define CYAN "\001\e[0;36m\002"
# define RESET "\001\e[0m\002"
# define MAX_CMD 500

typedef enum e_command
{
	argument,
	option,
	env,
	pwd,
	cd,
	echo,
	unset,
	dollar,
	question,
	e_export,
	e_exit,
	e_pipe,
	redirect_input,
	redirect_output,
	heredoc,
	append_redirect_output,
	not_comm,
	in_sys,
}							t_comm;

typedef struct s_token
{
	int						is_head;
	int						is_end;
	char					*token;
	t_comm					command;
	struct s_token			*next;
	struct s_token			*prev;
}							t_token;

typedef struct s_instruction
{
	int						id;
	t_token					*start;
	t_token					*end;
	struct s_instruction	*next;
	struct s_instruction	*prev;
}							t_instru;

typedef struct s_exec_iteration
{
	int						i;
	int						start;
	int						end;
	int						here_doc_fd[2];
	int						redir_in_fd;
	int						pipefd[2];
	int						check;
}							t_exec;

typedef struct s_var
{
	int						status;
	int						history;
	int						nb_command;
	int						current_status;
	char					*path_history;
	char					*hdoc_line;
	char					*line;
	char					**tab_env;
	char					**path;
	t_list					*env;
	t_instru				*instru;
	t_token					*token;
	t_exec					*iteration;
}							t_var;

int			ft_cd(char *path, t_var *var, char *var_name);
int			ft_export(t_var *var, char *to_add);
char		*ft_pwd(void);
void		ft_exit(t_var *var, long long end);
void		ft_echo(char *to_print, char option);
int			ft_unset_envp(t_var *var, char *to_del);
int			ft_exec_pwd(void);
int			ft_exec_env(t_var *var, t_token *nxt);
int			ft_exec_echo(t_token *start, t_token *end);
int			ft_exec_cd(t_var *var, t_token *start);
int			ft_exec_unset(t_var *var, t_token *start,
				t_token *end);
int			ft_exec_exit(t_var *var, t_token *start,
				t_token *end);
int			ft_exec_export(t_var *var, t_token *start,
				t_token *end);
int			ft_exec_echo_util(t_token *s, t_token *e, char tmp);
void		ft_quote_rdefine(t_token **start, t_token *end);
t_token		*ft_define_first_arg(t_token *start, t_token *end);
int			ft_dollar_aux(char *to_check, int i);
char		*ft_dq_redefine(char *to_check, char curr[2], char *join, int i);
int			ft_exec_cd_util(t_var *var, t_token *s, char *home,
				char *old);
void		ft_display_export(t_list *env);
void		ft_opt_echo(t_token **start);
int			ft_div_by_token(t_var *var);
void		ft_redirection(t_token *current, t_token *nxt);
void		ft_affect_dollar(t_token *current);
void		ft_parse_arg(t_token *current, t_token *nxt);
void		ft_parse(t_token *current);
char		*ft_del_quote(char *word, char *quote);
char		*ft_get_first_quote(char *str);
t_token		*ft_create_token(char *token);

char		*ft_expand(t_var *var, char *to_change, int do_exp);
char		*ft_expand_res(t_list *env, char *to_change,
				int start, int len);
char		*ft_exp(t_var *var, char *arg, int start,
				char *status);
void		ft_parse_sys(t_token *current, t_token *nxt);
char		*ft_expand_to_value(t_list *env, char *to_change);
int			ft_get_dollar_key(char *to_change, int start);
int			ft_check_case(char *to_process, char *arg);
char		*ft_exec_case(t_var *var, char *arg, int *i,
				int exec_case);
char		*ft_get_pid(void);
int			ft_exec_exp(char *to_check, int end, int do_exp);
char		*ft_define_quote(char *to_change);

int			ft_count_token_in_instru(t_instru *current);
void		ft_set_instru(t_instru **test, t_token *head);

int			ft_valid_exit(char *ref, long long value);
int			ft_valid_redirect_input(t_token *token);
char		*ft_rm_case(int q_case);
void		ft_exec(t_instru *instruction, t_var *var);
void		ft_check_access_directory(char *check_ambigous,
				t_token *target);
void		ft_handle_redirection_error(char **check_ambigous,
				t_token *target, t_var *var);
void		ft_handle_redirection(t_var *var, t_token *target,
				int *hd, int *input_redir);
void		ft_handle_exit_status(t_var *var);
void		ft_init_exec_current_instru(pid_t *pid);
void		ft_init_exec(t_exec *it, t_var *var);
void		ft_dup_fd_not_start_instru(t_exec it,
				int *input_fd);
void		ft_dup_fd_end_instru(t_exec it,
				int has_redirection);
char		ft_first_quote(char *word, char first, char second);
char		*ft_check_ambigous(t_var *var, t_token *token);
char		**ft_get_execve_parameter(t_instru *current);
int			ft_check_dir(char *path);
int			ft_exec_builtin(t_var *all_var, t_token *start,
				t_token *end);
int			ft_simul_heredoc(t_token *target, int hd,
				t_var *var);
int			ft_handle_unclosed_pipe(t_var *var, t_instru **tmp,
				t_exec *iteration);
int			ft_check_before_exec(t_instru *tmp, t_token *target,
				t_exec *iteration, t_var *var);
int			ft_valid_redir(t_token *target);
int			ft_exec_once(t_instru *tmp, t_var *var);
char		*ft_handle_error_dir(char **all_path, char **params,
				t_var *var);
char		*ft_handle_no_such_file(t_instru *tmp,
				char **all_path, char **params, t_var *var);
pid_t		ft_exec_once_builtin(t_instru *tmp, t_var *var,
				pid_t *pid);
t_token		*ft_find_redirection(t_token *token);
t_token		*ft_find_cmd_token(t_instru *instru);
t_token		*ft_find_cmd(t_token *start, t_comm to_find);
int			ft_redir_error(char *check_ambigous,
				t_token *target, int *output_fd);
int			ft_is_ambigous(t_comm type, char *to_check, int start);

void		ft_add_token(t_token **head, t_token *new_elem, int is_arg);
void		ft_command_setup(t_token **head);

void		ft_free_all(char **split);
void		ft_lstclear_shell(t_token **head);
void		ft_lstclear_instru(t_instru **instru,
				t_token **head);
void		ft_free_minishell(t_var *all_var);
int			ft_check_cmd(char *token);
int			ft_max_value(int val1, int val2);
int			ft_find_char(char *token, char to_find);
int			ft_is_char_pair(char *line, char quote);
int			ft_count_quote(char *str, char *to_count);
char		*ft_div_by_redirect(char *line, char *ref);
char		*ft_strdup_shell(char *s);
char		*ft_getvar(t_list *envp, char *var);
char		*ft_strjoin_shell(char *first, char *second);
char		*ft_env_variable(t_list *envp, char *to_find);
char		**ft_new_envp(t_list *env);
char		*ft_verify_exec_cmd(char **paths);
char		**ft_get_all_path(t_list *env, char *line);
char		**ft_split_shell(char *s, char c);
char		*ft_utils_split_aux(unsigned int *j, char *s);
void		ft_utils_count_c(char *p1, unsigned int *i);
void		ft_edit(char *line, int *i, char *result, int *j);
void		ft_div_aux(char *result, char div, int *j, int nb);
void		ft_handle_exit_status(t_var *var);
void		ft_exit_status(t_token *start, int *status,
				long long *value, int do_exit);
int			ft_handle_oldpwd(char *to_change, t_var *var,
				char *tmp, int show);
t_token		*ft_find_cmd(t_token *start, t_comm to_find);
int			ft_is_option(t_token *current, t_token *cmd);
int			ft_is_quote_closed(char *to_check);
int			ft_is_pipes_closed(char *to_check);
long long	ft_atoi_shell(char *str, int *status);
int			ft_count_special_char(t_token *token,
				t_comm to_count);
int			ft_handle_many_arg(t_token *start);
char		*ft_itoa_shell(long long nb);
void		ft_free_char(char **tmp);
int			ft_free_in_ambigous(t_var *var,
				char *check_ambigous);
void		ft_close_pipe(t_var *var);
int			ft_is_open(char *to_check, int end);
void		ft_rl_cut(t_var *var, char *to_add);
int			ft_init_pipe_hd(t_instru *tmp, t_var *var);
void		ft_create_envp(t_list **all_env, char **envp);
int			ft_export_aux(t_var *var, char *to_add);
int			ft_unset_utils(t_var *var, t_list **tmp,
				t_list **after, char *to_del);
void		ft_sort_env_list(t_list **result, char **tab_env);
void		ft_add_to_env(t_var *var, char *to_add,
				char *val_env, int ind_equal);
void		ft_upgrade_env(t_var *var, char *var_name,
				char *new_val);
int			ft_valid_export(char *to_add);

void		ft_minishell_core(t_var *var);
void		ft_display_env(t_list **env, char *before);

void		ft_display_error(char *error, char *token);
void		ft_display_error_without_exit(char *error,
				char *token, int status, t_var *var);
char		*ft_pipes_valid(t_var *var, char *line);
char		*ft_expand_hdoc(char *to_change, int *bool);
t_var		*ft_get_struct_var(void);
void		ft_simple_interrupt(int signal, siginfo_t *test, void *i);
void		ft_interupt_and_exit(int signal);
void		ft_handle_empty_com(t_instru *tmp, t_var *var);
int			ft_minishell_history(t_var *all_var, int do_append);
void		ft_history(t_var *all_var);
void		ft_check_point(t_instru *tmp, t_var *var);
void		ft_update_has_redirection(t_token *target, int *has_redirection);
char		*ft_list_to_not_expand(char *check);
char		*ft_expand_parse(t_var *var, char *to_exp, char *not_exp);
char		*ft_list_aux(char *check, int *i, char tmp, int *flag);
char		*ft_div_expand_aux(char *change, int *i, char tmp[2]);
int			ft_del_dollar(char *to_change, int ind_dollar);
int			ft_tkn_errors(t_var *var, char *to_tkn, char q_ref);
void		ft_define_exp_del_quote(char **exp, char *list, char *to_tkn);
void		ft_join_until_close_quote(char **result,
				char *check, int *i, char quote[2]);
int			ft_add_special(char curr[2],
				char *to_change, int *i, char **result);
int			ft_exit_error(t_token *arg);
int			ft_find_cmd_intru(t_instru *tmp, t_comm target);
void		ft_reinit_heredoc_fd(t_instru *tmp, t_token *target, t_var *var);

void		ft_show_list(t_list	*list);
t_list		*ft_divide_all(char *to_divide);
void		ft_apply(t_var *var, char *to_tkn, int is_arg, int is_value);
void	ft_display_token(t_token *token);
#endif
