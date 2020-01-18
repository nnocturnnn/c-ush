#include "ush.h"

int main(int argc, char **argv, char **envr) {
    char *input;
    int exit;
    char **commands;
    char **env;

    env = init_envr(argc, argv, envr);
	while (1) {
        mx_display();
        //signal ?
        mx_get_input(&input);
        if (mx_isemptystr(input, 1)) {
            free(input);
            continue;
        }
        commands = mx_strsplit(input, ';');
        free(input);
        exit = mx_exec_commands(commands);
        mx_del_strarr(commands);
        if (exit == -1) break;
	}
    mx_del_strarr(env);
}
