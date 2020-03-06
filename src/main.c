#include "ush.h"

void signal_handler(int signo) {
    extern char **environ;

    if (signo == SIGINT) {
		mx_printstr("\n");
        mx_display(environ);
        signal(SIGINT, signal_handler);
    } else if (signo == EOF) {
        mx_printstr("\n");
		signal(EOF, signal_handler);
    } else if (signo == SIGTSTP) {
        signal(SIGTSTP, signal_handler);
    }
}



static bool mx_isescape_char(char *input, int i) {
    if (i > 0 && input[i - 1] == '\\' && !mx_isescape_char(input, i - 1)) {
        return true;
    }
    if (i == 1 && input[i - 1] == '\\')
        return true;
    return false;
}

static int mx_skip_quotes(char *input, unsigned int *i, char c) {
    if (input[*i] == c && !mx_isescape_char(input, *i)) {
        (*i)++;
        while (input[*i]) {
            if (input[*i] == c
                && !mx_isescape_char(input, *i)) {
                break;
            }
            (*i)++;
        }
    return 1;
    }
    return 0;
}


t_list *mx_split_command(char *command) {
    t_list *result = NULL;
    unsigned int save = 0;

    for (unsigned int i = 0; i < strlen(command); i++) {
        mx_skip_quotes(command, &i, '\"');
        mx_skip_quotes(command, &i, '\'');
        if (isspace(command[i]) && !mx_isescape_char(command, i)) {
            save++;
            continue;
        }
        if (!command[i + 1] || (isspace(command[i + 1])
            && !mx_isescape_char(command, i + 1))) {
            mx_push_back(&result, strndup(command + save, i - save + 1));
            save = i + 1;
        }
    }
    return result;
}

static char *get_formated_arg(char *str) {
    char *result = mx_strnew(ARG_MAX);
    unsigned int len = strlen(str);
    unsigned int index = 0;
    bool is_quotes[2];

    for (unsigned int i = 0; i < len; i++) {
        if ((str[i] == '\'') && !mx_isescape_char(str, i) && !is_quotes[1]) {
            is_quotes[0] = !is_quotes[0];
            continue;
        }
        if ((str[i] == '\"') && !mx_isescape_char(str, i) && !is_quotes[0]) {
            is_quotes[1] = !is_quotes[1];
            continue;
        }
        if (mx_isescape_char(str, i + 1) && !is_quotes[0] && !is_quotes[1]) {
            i++;
        }
        result[index++] = str[i];
    }
    return result;
}

static char **get_result(char *command) {
    char **result = NULL;
    t_list *arguments = NULL;
    int len = 0;
    unsigned int i = 0;

    arguments = mx_split_command(command);
    len = mx_list_size(arguments);
    result = malloc(sizeof(char*) * (len + 1));
    result[len] = NULL;
    for (t_list *cur = arguments; cur; cur = cur->next) {
        result[i++] = get_formated_arg(cur->data);
    }
    mx_del_list(&arguments);
    mx_strdel(&command);
    return result;
}

char **mx_interpretate(char *command) {
    if (!strlen(command))
        return NULL;
    return get_result(command);
}

static int exec_commands(t_ush data, char ***env) {
    int i = -1;
    int exit = 0;
    char **command;

    while (data.commands[++i]) {
        if (mx_isemptystr(data.commands[i], 1)) {
            continue;
        }
        command = mx_interpretate(data.commands[i]);
        exit = mx_run_command(command, data, env, 1);
        if (exit != 1 && exit != 0)
            break;
    }
    mx_del_strarr(&command);
    return exit;
}

static t_ush *init(int argc, char **argv) {
    t_ush *data = malloc(sizeof(t_ush));
    char **var = (char **)malloc(sizeof(char *) * 100);//arr variable
   	char **alias = (char **)malloc(sizeof(char *) * 1000);//arr alias
   	char **commands = (char **)malloc(sizeof(char *) * 100);//arr commands

    data->logical = 0; // -1 &&    1 ||
    data->var = var;
    data->alias = alias;
    data->commands = commands;
    data->var[0] = strdup("?=0");
    return data;
}

static int circle_main(char **env, t_ush data) {
    char *input;
    int ret;

     while (1) {
        signal(SIGINT, signal_handler);
        if (isatty(0))
            mx_display(env);
        data.commands = mx_get_input(&input, data, &env);
        if (mx_isemptystr(input, 1)) {
            continue;
        }
        free(input);
        ret = exec_commands(data, &env);
        // mx_print_env(env);
        if (ret != 0 && ret != 1) 
            break;
	}
    return ret;
}

int main(int argc, char **argv, char **envr) {
    char **env;
    int q;

    // set_input_mode();
    env = mx_init_envr(argc, argv, envr);
    t_ush *data = init(argc, argv);
    free(data);
    q = circle_main(env, *data);
    mx_del_strarr(&env);
    // mx_reset_input_mode();
    exit(q);
}