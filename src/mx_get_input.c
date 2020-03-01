#include "ush.h"

static bool mx_isescape_char(char *input, int i) {
    if (i > 0 && input[i - 1] == '\\' && !mx_isescape_char(input, i - 1)) {
        return true;
    }
    if (i == 1 && input[i - 1] == '\\')
        return true;
    return false;
}

static int mx_skip_quotes(char *input, unsigned int *i, char c) {
    if (input[*i] == c && !mx_isescape_char(input, *i)) {
        (*i)++;
        while (input[*i]) {
            if (input[*i] == c
                && !mx_isescape_char(input, *i)) {
                break;
            }
            (*i)++;
        }
    return 1;
    }
    return 0;
}


static char *check_alias(char *input, t_ush data){
    int i = -1;

    while(data.alias[++i]) {
        if ((mx_get_substr_index(input,*mx_strsplit(data.alias[i],'='))) >= 0) {
            input = mx_replace_substr(input,mx_strdup(*mx_strsplit(data.alias[i],'=')),
            mx_get_env_var(mx_strdup(*mx_strsplit(data.alias[i],'=')), data.alias));
        }
    }
    return input;
}

void mx_skip_expansion(char *input, unsigned int *i) {
    int br = 0;

    if (input[*i] == '$' && !mx_isescape_char(input, *i)) {
        if (input[*i + 1] == '(' && !mx_isescape_char(input, *i)) {
            *i += 2;
            br++;
            while (input[*i]) {
                if (input[*i] == '(' && !mx_isescape_char(input, *i)) {
                    br++;
                }
                if (input[*i] == ')' && !mx_isescape_char(input, *i)) {
                    br--;
                }
                if (!br)
                    break;
                (*i)++;
            }
        }
    }
}

bool mx_check_quotes(char *input) {
    size_t len = strlen(input);
    bool s_qu = false;
    bool d_qu = false;

    for (unsigned int i = 0; i < len; i++) {
        mx_skip_quotes(input, &i, '`');
        mx_skip_expansion(input, &i);
        if (input[i] == '\"'
            && !mx_isescape_char(input, i) && !s_qu) {
            d_qu = !d_qu;
        }
        if (input[i] == '\''
            && !mx_isescape_char(input, i) && !d_qu) {
            s_qu = !s_qu;
        }
    }
    return !s_qu && !d_qu;
}

bool mx_check_brackets(char *c) {
    unsigned int len = strlen(c);
    int stack[len];
    int top = -1;

    for (unsigned int i = 0; i < len; i++) {
        mx_skip_quotes(c, &i, '`');
        if ((c[i] == '(' && !mx_isescape_char(c, i))
            || (c[i] == '{' && !mx_isescape_char(c, i))
            || (c[i] == ')' && !mx_isescape_char(c, i))
            || (c[i] == '}' && !mx_isescape_char(c, i))) {
            top++;
            stack[top] = c[i];
        }
        if ((stack[top] == ')' && (top - 1 >= 0 && stack[top - 1] == '('))
            || (stack[top] == '}' && (top - 1 >= 0 && stack[top - 1] == '{')))
            top = top - 2;
    }
    return top == -1;
}

bool mx_check_substitutions(char *command) {
    bool g_quotes = false;
    size_t len = strlen(command);
    
    for (unsigned int i = 0; i < len; i++) {
        mx_skip_quotes(command, &i, '\'');
        mx_skip_expansion(command, &i);
        if (command[i] == '`' && !mx_isescape_char(command, i)) {
            g_quotes = !g_quotes;
        }
    }
    return !g_quotes;
}

int mx_checkclosequots(char *input){
    int result = 0;
    if (!(result = mx_check_quotes(input))){
        fprintf(stderr, "%s: syntax error: missing terminating character\n", 
        "ush");
    }
    else if (!(result = mx_check_brackets(input))) {
        fprintf(stderr, "%s: syntax error: missing brace character\n", 
        "ush");
    }
    else if (!(result = mx_check_substitutions(input))) {
        fprintf(stderr, "%s: syntax error: missing subsitution character\n", 
        "ush");
    }
    return result;
}


static int get_next_command(char *command) {
    for (unsigned int i = 0; i < strlen(command); i++) {
        mx_skip_quotes(command, &i, '`');
        mx_skip_quotes(command, &i, '\'');
        mx_skip_quotes(command, &i, '\"');
        mx_skip_expansion(command, &i);
        if (command[i] == ';')
            return i;
        if (!command[i + 1] && command[i] != ';')
            return i + 1;
    }
    return -1;
}

static t_list *split(char *command) {
    t_list *commands = NULL;
    int len = 0;
    char *tmp_cmd = strdup(command);
    char *save = tmp_cmd;
    
    for (unsigned int i = 0; len != -1; i++) {
        len = get_next_command(tmp_cmd);
        mx_push_back(&commands, strndup(tmp_cmd, len));
        if ((tmp_cmd[len] == ';' && !tmp_cmd[len + 1]) || !tmp_cmd[len])
            break;
        tmp_cmd += len + 1;
    }
    mx_strdel(&save);
    return commands;
}



char **mx_split_commands(char *command) {
    t_list *commands = split(command);
    size_t size = mx_list_size(commands);
    char **cmds = malloc(sizeof(char *) * (size + 1));
    unsigned int index = 0;

    cmds[size] = NULL;
    for (t_list *cur = commands; cur; cur = cur->next) {
        cmds[index++] = strdup(cur->data);
    }
    mx_del_list(&commands);

  
    return cmds;
}

static char **mx_parse_input(char *input, t_ush data, char ***env) {
    char *rep = check_alias(mx_replace_substr(input, "&&", ";"),data);
    char *nah_tild = mx_replace_substr(rep,"~",mx_get_env_var("HOME", *(env)));
    char **commands = mx_split_commands(nah_tild);
    
    if (!(mx_checkclosequots(input))){
        commands[0] = NULL;
    }
    for (int k = 0; commands[k] != NULL; k++){
        commands[k] = strdup(mx_strtrim(commands[k]));
        if (mx_strlen(commands[k]) == 1 && commands[k][0] == '\\') {
            mx_printstr("ush: \\ not closen \n");
            commands[k] = NULL;
        }
    }
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
        // mx_printchar(b);
        *(*input + i++) = b;
		*input = mx_realloc(*input, count + 1);
		count++;
    }
    *(*input + i) = '\0';
    if (!ret) {
		free(*input);
		mx_exit_shell(*(env));
	}
    parse_input = mx_parse_input(*input, data, env);
    return parse_input;
}