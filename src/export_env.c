#include "ush.h"

int mx_find_env_var(char *var,char **env) {
    int i = -1;
    char *tmp = mx_strjoin(var, "=");
    
    while (env[++i]) {
        if (mx_get_substr_index(env[i], tmp) == 0) {
            mx_strdel(&tmp);
            return i;
        }    
    }
    mx_strdel(&tmp);
    return i;
}


void mx_set_env_var(char *key, char *value, char ***env) {
	int	pos = mx_find_env_var(key, *env);;
	char *tmp = mx_strjoin("=", value);

	if (*(*env + pos)) {
		free(*(*env + pos));
		if (value)
			*(*env + pos) = mx_strjoin(key, tmp);
		else
			*(*env + pos) = mx_strjoin(key, "=");
	} else {
		*(env) = realloc_envv(pos + 1, *env);
		if (value)
			*(*env + pos) = mx_strjoin(key, tmp);
		else
			*(*env + pos) = mx_strjoin(key, "=");
	}
	free(tmp);
}

int mx_export_builtin(char **arg, t_ush data, char ***env) {
    int i = -1;
    char **sorted_env = mx_init_export(*env);
    
    if (!arg[0]) {
        mx_quicksort(sorted_env, 0, mx_sizearr(*env));
        mx_print_env(sorted_env);
        mx_del_strarr(&sorted_env);
        return 0;
    }
    while (arg[++i]) {
        if (mx_get_char_index(arg[i], '=') == 0 && mx_strlen(arg[i]) == 1) {
            mx_errors(USH_BAD_ASSIGN, ":(");
            return 1;
        } else {    
            mx_set_env_var(*mx_strsplit(arg[i],'='),
                           *(mx_strsplit(arg[i],'=') + 1), env);
        }
    }
	return 0;
}