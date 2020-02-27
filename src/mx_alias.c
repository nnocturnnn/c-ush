#include "ush.h"

int mx_alias(char **arg, char **data, char **env) {
    int i = -1;
    int j = -1;

    while (data[++j])
        j++;
    if (!arg[0]) {
        mx_print_env(data);
    }
    else {
        while(arg[++i]) {
            mx_set_env_var()
        }
    }
    return 1;
}
