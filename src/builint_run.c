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
    // else if (!mx_get_substr_index(command[0], "unalias"))
    else if (!mx_get_substr_index(command[0],"set")) {
        mx_print_env(data.var);
        return 1;
    } else
        return 0;
}

int check_builtins(char **command, t_ush data, char ***env) {
    int q = check_b1(command,data,env);

    if (q != 1 && q != 0) 
        return q;
    else
        if (check_b2(command,data,env) != 1)
            if (check_b3(command,data,env) != 1)
                return 0;
    return 1;
}