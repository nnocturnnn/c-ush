#include "ush.h"

static int which_check_built(char *flag,char *command) {
    int i = -1;
    char *builtin[] = {"exit","alias","cd","echo","export","unset","which",
                       "pwd","env","fg", NULL};
    while (builtin[++i] != NULL) {
        if (mx_strequ(command,builtin[i])) {
        	if (mx_strequ(flag, "-s")) {
	            mx_printstr(command);
	            mx_printstr(": ush built-in command");
	            mx_printstr("\n");
        	}
            return 1;
        }
    }
    return 0;
}


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

int mx_which_builtin(char **arg, char **env) {
    int	i = -1;
    int q = 0;
	char *bin_path;
	char **path = mx_strsplit(mx_get_env_var("PATH", env), ':');
	struct stat	f;

    while (arg[++q]) {
        if (!which_check_built(arg[0],arg[q]) || mx_strequ(arg[0],"-a")) {
            while (path && path[++i]) {
                bin_path = mx_pathjoin(path[i], arg[i]);
                if (access(bin_path, F_OK) != -1 && !mx_strequ(arg[0],"-s"))
                    mx_printstr(bin_path);
                return 0;
            }
        }
    }
	// free(path);
	return 1;
}
