#include "ush.h"

void print_path(char *path, char **env) {
	char *parsed_home = mx_parse_home_path(path, 0, env);

	mx_printstr(parsed_home);
	mx_strdel(&parsed_home);
}

void mx_print_env(char **env) {
    int i = 0;
    
    if (env)
        while (env[i]) {
            mx_printstr(env[i++]);
            mx_printstr("\n");
        }
}
