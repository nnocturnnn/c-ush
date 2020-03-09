#include "ush.h"

char **mx_init_envr(int argc, char **argv, char **envr) {
    char **env = (char **)malloc(sizeof(char *) * 100);
    int i = -1;
    char buff[4096 + 1];
    char *cwd = getcwd(buff, 4096);

    while(envr[++i]) {
        if (!(env[i] = strdup(envr[i])))
            exit(255);
    }
    if(envr[0] != NULL) {
        mx_set_var("SHLVL",mx_itoa(atoi(mx_get_env_var("SHLVL",env)) + 1),env);
     } else {
        env[0] = strdup("SHLVL=1");
        env[1] = strdup(mx_strjoin("PWD=",cwd));
        env[2] = NULL;
    }
    return env;
}

char **mx_init_export(char **env) {
    char **export = (char **)malloc(sizeof(char *) * (envv_len(env) +1));
    int i = -1;

    while(env[++i]) {
        if (!(export[i] = strdup(env[i])))
            exit(255);
    }
    export[i] = NULL;
    return export;
}