#include "ush.h"

static char *check_alias(char *input, t_ush data){
    int i = -1;

    while(data.alias[++i]) {
        if ((mx_get_substr_index(input,
             *mx_strsplit(data.alias[i], '='))) == 0) {
            input = mx_replace_substr(input, 
            mx_strdup(*mx_strsplit(data.alias[i],'=')),
            mx_get_env_var(mx_strdup(*mx_strsplit(data.alias[i], '=')),
            data.alias));
        }
    }
    return input;
}

static char *mx_get_var_input(char *input, char **var) {
    int i = -1;
    int c = -1;
    char **commands = mx_strsplit(input,' ');

    while (commands[++i]) 
        if (mx_get_char_index(commands[i],'=') == -1) {
            while (commands[++c] && mx_get_char_index(commands[c],'=') != -1) {
                input = mx_replace_substr(input,
                                         get_word_by_char(commands[c],'='),"");
            }
            return input;
        }
    while (commands[++c]){
        mx_set_var(*mx_strsplit(commands[c],'='),*(mx_strsplit(commands[c],
                                                               '=') + 1),var);
        input = mx_replace_substr(input,get_word_by_char(commands[c],'='),"");
    }
    mx_del_strarr(&commands);
    return input;
}

char *replace_tild(char *rep, char **env, int *i) {
    if (mx_get_char_index(rep, '~') < 0)
        return rep;
    int it = mx_get_char_index(rep, '~');

    i++;
    rep = mx_replace_substr(rep, "~+", mx_get_env_var("PWD", env));
    rep = mx_replace_substr(rep,"~-",mx_get_env_var("OLDPWD", env));
    if (rep[it + 1] == ' ' || rep[it + 1] == '\0' || rep[it + 1] == '/')
        rep = mx_replace_substr(rep, "~", mx_get_env_var("HOME", env));
    else
        rep = mx_replace_substr(rep, "~", "/Users/");

    return rep;
}

char *logical(char *str, char **var) {
    if (mx_get_substr_index(str, "&&") > -1)
        mx_set_var("logical","&&",var);
    if (mx_get_substr_index(str, "||") > -1)
        mx_set_var("logical","||",var);
    char *st = mx_replace_substr(mx_replace_substr(str, "&&", "^"),"||", "^");
    return st;
}

static char **mx_parse_input(char *input, t_ush data, char ***env) {
    int codetilda = 0;
    char *rep = check_alias(logical(input,data.var), data);
    char *nah_tild = replace_tild(rep, *env, &codetilda);
    char **commands = mx_split_commands(nah_tild);

    if (!(mx_checkclosequots(input)) || (mx_checkdotkoma(input)))
        commands[0] = NULL;
    for (int k = 0; commands[k] != NULL; k++){
        commands[k] = strdup(mx_strtrim(mx_get_var_input(commands[k],
                                                         data.var)));
        if (mx_strlen(commands[k]) == 1 && commands[k][0] == '\\') {
            mx_printstr("ush: \\ not closen \n");
            commands[k] = NULL;
        }
    }
    mx_strdel(&rep);
    if (codetilda)
        mx_strdel(&nah_tild);
    return commands;
}



char **mx_get_input(char **input, t_ush data, char ***env) {
    int ret;
    char b;
    int i = 0;
    char **parse_input;
    int count = 1;

    *input = mx_strnew(1);
    while ((ret = read(0, &b, 1)) && b != '\n') { 
        *(*input + i++) = b;
		*input = mx_realloc(*input, count + 1);
		count++;
    }
    *(*input + i) = '\0';
    if (!ret) {
		exit(0);
	}
    parse_input = mx_parse_input(*input, data, env);
    return parse_input;
}
