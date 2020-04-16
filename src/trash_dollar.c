#include "ush.h"

char **mx_rep(char **command, char **env, char **var) {
    for (int k = 0; command[k] != NULL; k++)
        command[k] = mx_replace_dolars(command[k], &env, var);
    return command;
}

char *repwithoutskob(char *n, char ***env, char **s, int *z) {
    if (mx_get_env_var(strndup(n + z[0], z[2]) + 1, *(env)) != NULL)
        n = rec_cord(n,
                     strndup(n + z[0], z[2] - 1),
                     mx_get_env_var(strndup(n + z[0], z[2]) + 1, *(env)),
                     z[0]);
    else if (mx_get_env_var(strndup(n + z[0], z[2]) + 1, s) != NULL)
        n = rec_cord(n, 
                     strndup(n + z[0], z[2] - 1),
                     mx_get_env_var(strndup(n + z[0], z[2]) + 1, s), z[0]);
    else
        n = rec_cord(n, strndup(n + z[0], z[2] - 1), "", z[0]);
    return n;
}

char *repwithskob(char *n, char ***env, char **s, int *z) {
    if (mx_get_env_var(strndup(n + z[0], z[2] - 1) + 2, *(env)) != NULL)
        n = rec_cord(n,
                     strndup(n + z[0], z[2] - 1),
                     mx_get_env_var(strndup(n + z[0], z[2] - 1) + 2, *(env)),
                     z[0]);
    else if (mx_get_env_var(strndup(n + z[0], z[2] - 1) + 2, s) != NULL)
        n = rec_cord(n,
                     strndup(n + z[0], z[2] - 1),
                     mx_get_env_var(strndup(n + z[0], z[2] - 1) + 2, s),
                     z[0]);
    else
        n = rec_cord(n, strndup(n + z[0], z[2] - 1), "", z[0]);
    return n;
}
