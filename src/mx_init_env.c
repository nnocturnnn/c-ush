#include "ush.h"

char **mx_init_envr(int argc, char **argv, char **envr) {
    char **env;
    int i = -1;

    (void)argc;
    (void)argv;
    env = (char **)malloc(sizeof(char *) * (envv_len(envr) +1));
    while(envr[++i]) {
        if (!(env[i] = mx_strdup(envr[i])))
            mx_exit_shell(env);
    }
    return env;
}
