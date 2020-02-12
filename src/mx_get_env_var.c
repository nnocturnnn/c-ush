#include "ush.h"

char **realloc_envv(int new_size, char **env) {
    int i = -1;
	char **new = (char **)malloc(sizeof(char *) * (new_size + 1));;

	while (env[++i] && i < new_size) {
		new[i] = mx_strdup(env[i]);
		free(env[i]);
	}
	free(env);
	return new;
}

int envv_len(char **envv) {
	int	i = -1;
	int	count = 0;
    
	while (envv[++i])
		count++;
	return (count);
}

char *mx_get_env_var(char *var, char **env) {
    char *tmp;
    int i = -1;

    while (env[++i]) {
        tmp = mx_strjoin(var, "=");
        if ((mx_get_substr_index(env[i], tmp)) == 0) {
            free(tmp);
            return (mx_strchr(env[i], '=') + 1);
        }
        free(tmp);
    }
    return NULL;
}

void mx_remove_env_var(int var_pos, char **env) {
    int i;
    int var_count;

    mx_strdel(&env[var_pos]);
    i = var_pos;
	var_count = var_pos + 1;
	while (env[i + 1])
	{
		env[i] = mx_strdup(env[i + 1]);
		free(env[i + 1]);
		i++;
		var_count++;
	}
	env = realloc_envv(var_count - 1, env);
}
