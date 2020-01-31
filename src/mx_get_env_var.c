#include "ush.h"

char *mx_get_env_var(char *var, char **env) {
    char *tmp;
    int i = -1;

    while (env[++i]) {
        tmp = mx_strjoin(var, "=");
        if ((mx_get_substr_index(env[i], tmp)) > -1) {
            free(tmp);
            return (mx_strchr(env[i], '=') + 1);
        }
        free(tmp);
    }
    return NULL;
}
