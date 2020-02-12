#include "ush.h"

int mx_pwd_builtin(char **arg, char **env) {
    char *cwd;
    char buff[4096 + 1];

    if (!arg[0] || mx_strequ(arg[0], "-L")) {
        mx_printstr(mx_get_env_var("PWD", env));
        mx_printstr("\n");
        return 1;
    } else {
        cwd = getcwd(buff, 4096);
        mx_printstr(cwd);
        mx_printstr("\n");
        return 1;
    }
}
