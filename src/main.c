#include "ush.h"

void signal_handler(int signo) {
	if (signo == SIGINT) {
		mx_printstr("\n");
		signal(SIGINT, signal_handler);
	} else if (signo == EOF) {
        mx_printstr("\n");
		signal(EOF, signal_handler);
    } else if (signo == SIGTSTP) {
        signal(SIGTSTP, signal_handler);
    }
}

static int exec_commands(t_ush data, char ***env) {
    int i = -1;
    int exit = 0;
    char **command;

    while (data.commands[++i]) {
        command = mx_strsplit(data.commands[i], ' ');
        exit = mx_run_command(command, data, env, 1);
        //free(&command);
        if (exit == 1)
            break;
    }
    return exit;
}

static t_ush *init(int argc, char **argv) {
    t_ush *data = malloc(sizeof(t_ush));
    char **var = (char **)malloc(sizeof(char *) * 100);//arr variable
   	char **alias = (char **)malloc(sizeof(char *) * 100);//arr alias
   	char **commands = (char **)malloc(sizeof(char *) * 100);//arr commands
    char **history = (char **)malloc(sizeof(char *) * 100);

    data->count_var = 0;
    data->var = var;
    data->alias = alias;
    data->commands = commands;
    data->history = history;
    return data;
}

static void circle_main(char **env, t_ush data) {
    char *input;
    int ret;

     while (1) {
        // key_handler(data);
        signal(SIGINT, signal_handler);
        mx_display(env);
        data.commands = mx_get_input(&input, data, &env);
       // mx_save_history(input, data);
        if (mx_isemptystr(input, 1)) {
            free(input);
            continue;
        }
        //free(input);
        ret = exec_commands(data, &env);
        // free(&commands);
        if (ret == -1) 
            break;
	}
}

int main(int argc, char **argv, char **envr) {
    char **env;

    // set_input_mode();
    env = mx_init_envr(argc, argv, envr);
    t_ush *data = init(argc, argv);
    circle_main(env, *data);
    // free(&env);
    // mx_reset_input_mode();
    exit(1);
}
