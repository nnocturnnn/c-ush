#include "ush.h"

int mx_alias(char **arg, char **data, char **env) {
    int i = -1;
    int j = 0;

    while (data[j])
        j++;
    if (!arg[0]) {
        mx_print_env(data);
    }
    else {
        while(arg[++i]) {
            data[j] = strdup(arg[i]);
            j++;
        }
    }
    return 1;
}
