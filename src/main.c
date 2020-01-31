#include "ush.h"

void	signal_handler(int signo, char **env) {
	if (signo == SIGINT) {
		mx_printstr("\n");
		mx_display(env);
		signal(SIGINT, signal_handler(signo, env));
	}
}

void	proc_signal_handler(int signo) {
	if (signo == SIGINT) {
        mx_printstr("\n");
		signal(SIGINT, proc_signal_handler);
	}
}


int main(int argc, char **argv, char **envr) {
    char *input;
    int exit;
    char **commands;
    char **env;

    env = mx_init_envr(argc, argv, envr); 
    while (1) {
        mx_display(env);
        signal(SIGINT, signal_handler);
        commands = mx_get_input(&input, env);
        if (mx_isemptystr(input, 1)) {
            free(input);
            continue;
        }
        free(input);
        exit = mx_exec_commands(commands);
        // mx_del_strarr(commands);
        // if (exit == -1) 
        //     break;
	}
    // mx_del_strarr(&env);
}
