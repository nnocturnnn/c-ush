#include "ush.h"

static char *check_alias(char *input, t_ush data){
    int i = -1;

    while(data.alias[++i]) {
        if ((mx_get_substr_index(input,*mx_strsplit(data.alias[i],'='))) >= 0)
            input = mx_replace_substr(input,mx_strdup(*mx_strsplit(data.alias[i],'=')),
            mx_get_env_var(mx_strdup(*mx_strsplit(data.alias[i],'=')), data.alias));
    }
    return input;
}

// static void get_var(char *line, t_ush data) {
//     int count = mx_count_char(line, '=');
    
//     for (int i = data.count_var;i < data.count_var + count; i++) {
//         if (!mx_isspace(line[mx_get_char_index(line, '=') - 1]))
//             data.var[i] = mx_strdup()
//     }
//     data.count_var += count;
    
// }

// static char **best_parse(char **arg, t_ush data){
//     int i = -1;
//     int q = 0;

//     while (arg[++i]) {
//         get_var(arg[i],data);
//     }
// }

static char **mx_parse_input(char *input, t_ush data, char ***env) {
    char *rep = check_alias(mx_replace_substr(input, "&&", ";"),data);
    char *nah_tild = mx_replace_substr(rep,"~",mx_get_env_var("HOME", *(env)));
    char **split_str_by_sep = mx_strsplit(nah_tild,';');
    char **without_space = (char **)malloc(sizeof(char *) * 1000);
    int i = -1;

    while (split_str_by_sep[++i]){
        without_space[i] = mx_strdup(mx_del_extra_spaces(split_str_by_sep[i]));
    }
    without_space[i] = NULL;
    return /*best_parse(*/without_space/*, data)*/;
}



char **mx_get_input(char **input, t_ush data, char ***env) {
    int ret;
    char b;
    int i = 0;
    char **parse_input;
    int count = 1;

    *input = mx_strnew(1);
    while ((ret = read(0, &b, 1)) && b != '\n') { 
        // mx_printchar(b);
        *(*input + i++) = b;
		*input = mx_realloc(*input, count + 1);
		count++;
    }
    // write(1,"\n",1);
    *(*input + i) = '\0';
    if (!ret) {
		free(*input);
		mx_exit_shell(*(env));
	}
    parse_input = mx_parse_input(*input, data, env);
    return parse_input;
}