#include "ush.h"

int mx_alias(char **arg, t_ush data, char **env) {
    int i = -1;
    int j = -1;

    while (data.alias[++j])
        j++;
    if (!arg[0]) {
        mx_print_env(data.alias);
    }
    else {
        while(arg[++i]) {
            data.alias[j] = strdup(arg[i]);
            j++;
        }
    }
    return 1;
}
