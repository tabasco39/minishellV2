NAME = minishell

LIB = ./libft/libft.a

SRC = ft_program.c \
	  ft_history.c \
	  ft_minishell_core.c \
	  libft/get_next_line/get_next_line.c \
  	  libft/get_next_line/get_next_line_utils.c \
	  Utils/ft_utils.c \
	  Utils/ft_clear.c \
	  Utils/ft_utils_two.c \
	  Utils/ft_handle_path.c \
	  Utils/ft_utils_three.c \
	  Utils/ft_split_utils.c \
	  Execution/Made_exec/ft_executor.c \
	  Execution/ft_utils.c \
	  Execution/Builtins/ft_special_char.c \
	  Execution/Builtins/ft_exec_cmd.c \
	  Execution/Builtins/ft_builtins.c \
	  Execution/Builtins/ft_builtins_utils.c \
	  Execution/Builtins/ft_builtins_env.c \
	  Execution/Builtins/ft_utils.c \
	  Execution/Here_doc/ft_expand_hdoc.c\
	  Execution/Here_doc/ft_simul_hdoc.c\
	  Execution/Redirection/ft_utils.c\
	  Execution/Redirection/ft_utils_01.c\
	  Execution/Redirection/ft_handle_redirection.c\
	  Execution/Made_exec/ft_utils_03.c\
	  Execution/Made_exec/ft_utils_02.c\
	  Execution/Made_exec/ft_utils_01.c\
	  Execution/Made_exec/ft_utils.c\
	  Execution/Made_exec/ft_check_until_exec.c\
	  Env/ft_handle_envp.c \
	  Env/ft_new_envp.c \
	  Env/ft_env_utils.c \
	  Parsing/lexing/ft_token_control.c \
	  Parsing/lexing/ft_lexing.c \
	  Parsing/lexing/ft_edit.c \
	  Parsing/lexing/ft_utils.c \
	  Parsing/lexing/ft_div_by_token.c \
	  Parsing/lexing/ft_del_str.c \
	  Parsing/parse/ft_parse.c \
	  Parsing/parse/ft_parse_utils.c \
	  Parsing/parse/ft_get_pid.c \
	  Parsing/parse/ft_parse_dollar.c \
	  Parsing/parse/ft_parse_dollar_utils.c \
	  Parsing/parse/ft_define_quote.c \
	  Parsing/parse/ft_quote_util.c \
	  Parsing/instruction/ft_set_instructions.c \
	  Parsing/instruction/ft_utils.c

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror

all: ${NAME}

${NAME}: ${OBJ} ${LIB}
	${CC} ${CFLAGS} ${OBJ} ${LIB} -lreadline -o ${NAME}

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<

${LIB}:
	make -C ./libft/ bonus

clean:
	rm -f ${OBJ}
	make -C ./libft/ clean

fclean: clean
	rm -f ${NAME}
	make -C ./libft/ fclean

re: fclean all

.PHONY: all clean fclean re