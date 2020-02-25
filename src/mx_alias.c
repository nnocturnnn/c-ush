#include "ush.h"

int mx_alias(char **arg, t_ush data, char **env) {
    int i = -1;

    if (!arg[0]) {
        mx_print_env(data.alias);
    }
    else {
        while(arg[++i]) {
            data.alias[i] = mx_strdup(arg[i]);
        }
    }
    return 1;
}
