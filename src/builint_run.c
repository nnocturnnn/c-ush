#include "ush.h"

static int check_b1(char **command, t_ush data, char ***env){
    if (!mx_get_substr_index(command[0], "exit")) {
		return mx_exit_builtin(command + 1);
	} else if (!mx_get_substr_index(command[0], "bye")) {
        return bye_builtin(command);
    } else if (!mx_get_substr_index(command[0], "echo")) {
        mx_set_var("?",mx_itoa(mx_echo_builtin(command , data)),data.var);
		return 1;
    } else if (!mx_get_substr_index(command[0], "alias")) {
        mx_set_var("?",mx_itoa(mx_alias(command + 1, data.alias, *env)),
                   data.var);
        return 1;
    } else if (!mx_get_substr_index(command[0], "cd")) {
        mx_set_var("?",mx_itoa(mx_cd_builtin(command + 1, *env)),data.var);
		return 1;
    } else 
        return 0;
}

static int check_b2(char **command, t_ush data, char ***env) {
    if (!mx_get_substr_index(command[0], "export")){
        mx_set_var("?",mx_itoa(mx_export_builtin(command + 1, data, env)),
                   data.var);
		return 1;
	} else if (!mx_get_substr_index(command[0], "unset")){
       mx_set_var("?",mx_itoa(mx_unsetenv_builtin(command + 1, env)),data.var);
		return 1;
    } else if (!mx_get_substr_index(command[0], "which")) {
        mx_set_var("?",mx_itoa(mx_which_builtin(command + 1, *env)),data.var);
        return 1;
    } else if (!mx_get_substr_index(command[0], "pwd")) {
        mx_set_var("?",mx_itoa(mx_pwd_builtin(command + 1, *env)),data.var);
        return 1;
    } else if (!mx_get_substr_index(command[0], "env")) {
        mx_set_var("?",mx_itoa(mx_env_builtin(command + 1, data, *(env))),
                   data.var);
		return 1;
    } else 
        return 0;
}

static int check_b3(char **command, t_ush data, char ***env){
    if (!mx_get_substr_index(command[0], "True"))
        return true_builtin(data.var);
    else if (!mx_get_substr_index(command[0], "env")) {
        mx_set_var("?",mx_itoa(mx_env_builtin(command + 1, data, *(env))),
                   data.var);
		return 1;
    } else if (!mx_get_substr_index(command[0], "False"))
        return false_builtin(data.var);
    else if (!mx_get_substr_index(command[0],"fg")) {
        //command
		return 1;
	} else if (!mx_get_substr_index(command[0],"return"))
		return return_builtin(data.var, command + 1);
    else if (!mx_get_substr_index(command[0], "unalias")) {
        mx_set_var("?",mx_itoa(mx_unalias(command + 1, data.alias, *env)),
                   data.var);
        return 1;
    } else if (!mx_get_substr_index(command[0],"set")) {
        mx_print_env(data.var);
        return 1;
    } else
        return 0;
}

static char **getvars(char *nah_tild) {
    const char h = ' ';
    char **vars = (char **)malloc(sizeof(char *) * (mx_count_words(nah_tild, h)) + 5);
    int counter = 0;
    
    for (int i = 0; i < strlen(nah_tild); i++) {
        if (nah_tild[i] == '$' && nah_tild[i + 1] != '(') {
            int arr[2];

            arr[0] = i;
            while (nah_tild[i] != ' ' && i < strlen(nah_tild) 
                   && nah_tild[i - 1] != '}')
                i++;
            arr[1] = i;
            vars[counter++] = mx_strndup(nah_tild + arr[0], arr[1] - arr[0]);
        }
    }
    vars[counter] = NULL;
    return vars;
}



char *mx_replace_dolars(char *nah_tild, char ***env) {
    char **a = getvars(nah_tild); // nado free

    for (int i = 0; a[i] != NULL; i++) {
        if (a[i][1] != '{' ) {
            if ((mx_get_env_var(a[i] + 1, *(env)) != NULL))
                nah_tild = mx_replace_substr(nah_tild, a[i],
                                             mx_get_env_var(a[i] + 1, *(env)));
            else
                nah_tild = mx_replace_substr(nah_tild, a[i], "");
        }
        if(a[i][1] == '{') {
            if (mx_get_env_var(strndup(a[i] + 2,
                strlen(a[i]) - 3), *(env)) != NULL)
                nah_tild = mx_replace_substr(nah_tild, a[i],
                mx_get_env_var(strndup(a[i] + 2, strlen(a[i]) - 3), *(env)));
            else
                nah_tild = mx_replace_substr(nah_tild, a[i], "");
        }
    }
    mx_del_strarr(&a);
    return nah_tild;
}


char **mx_rep(char **command, char **env) {
    for (int k = 0; command[k] != NULL; k++)
        command[k] = mx_replace_dolars(command[k], &env);
    return command;
}

int check_builtins(char **command, t_ush data, char ***env) {
    command = mx_rep(command, *(env));
    int q = check_b1(command,data,env);

    if (q != 1 && q != 0) 
        return q;
    else
        if (check_b2(command,data,env) != 1)
            if (check_b3(command,data,env) != 1)
                return 0;
    return 1;
}