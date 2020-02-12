#include "ush.h"

int mx_find_env_var(char *var,char **env) {
    int i = -1;
    char *tmp = mx_strjoin(var, "=");
    
    while (env[++i]) {
        if (mx_get_substr_index(env[i], tmp) > -1) {
            // free(tmp);
            return i;
        }
        // free(tmp);
    }
    return i;
}

void mx_set_env_var(char *key, char *value, char **env) {
	int	pos = mx_find_env_var(key, env);;
	char *tmp = mx_strjoin("=", value);

	if (env[pos]) {
		free(env[pos]);
		if (value)
			env[pos] = mx_strjoin(key, tmp);
		else
			env[pos] = mx_strjoin(key, "=");
	} else {
		env = realloc_envv(pos + 1, env);
		if (value)
			env[pos] = mx_strjoin(key, tmp);
		else
			env[pos] = mx_strjoin(key, "=");
	}
	free(tmp);
}

int mx_export_builtin(char **arg, char **env) {
    int i = -1;
    char **key_val;

    if (!arg[0]) {
        mx_quicksort(env, 0, mx_sizearr(env));
        mx_print_env(env);
        return 1;
    }
    while (arg[++i]) {
        if (mx_get_char_index(arg[i], '=') == 0 && mx_strlen(arg[i]) == 1)
            mx_errors(USH_BAD_ASSIGN, ":(");
        else {
            char **key_val = mx_strsplit(arg[i], '=');
            mx_set_env_var(key_val[0],key_val[1], env);
        }
    }
	return 1;
}


int mx_unsetenv_builtin(char **arg, char **env) {
    int	i;
	int	var_pos;

	if (!arg[0]) {
		mx_errors(UNSET_ERR, "uns");
		return 1;
	}
	i = -1;
	while (arg[++i]) {
		var_pos = mx_find_env_var(arg[i], env);
		if (env[var_pos])
			mx_remove_env_var(var_pos, env);
	}
	return (1);
}
