#include "ush.h"

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

int mx_checkclosequots(char *input) {
    int result = 0;
    if (!(result = mx_check_quotes(input))){
        mx_printerr("ush: syntax error: missing terminating character\n");
    }
    else if (!(result = mx_check_brackets(input))) {
        mx_printerr("ush: syntax error: missing brace character\n");
    }
    else if (!(result = mx_check_substitutions(input))) {
        mx_printerr("ush: syntax error: missing subsitution character\n");
    }
    return result;
}

void mx_set_var(char *key, char *value, char **var) {
    char *tmp = mx_strjoin("=", value);
    int	pos;

    if (mx_get_char_index(key,'=') > 0)
        key[mx_strlen(key) - 1] = '\0';
	pos = mx_find_env_var(key, var);
	if (var[pos]) {
		free(var[pos]);
		if (value)
			var[pos] = mx_strjoin(key, tmp);
		else
			var[pos] = mx_strjoin(key, "=");
	} 
    else {
		if (value)
			var[pos] = mx_strjoin(key, tmp);
		else
			var[pos] = mx_strjoin(key, "=");
	}
	free(tmp);
}
