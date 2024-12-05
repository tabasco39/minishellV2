if [ "$1" == "m" ];
then
    make
    clear
    valgrind --leak-check=full --track-origins=yes --track-fds=yes --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
fi
if [ "$1" == "b" ];
then
    make bonus
    clear
    valgrind --leak-check=full --track-origins=yes --track-fds=yes --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell_bonus
fi