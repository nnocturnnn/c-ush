#include "ush.h"

static char **getvars(char *nah_tild) {
    char **vars = (char **)malloc(sizeof(char *) 
                    * (mx_count_words(nah_tild, ' ')) + 5);
    int counter = 0;
    
    for (int i = 0; i < strlen(nah_tild); i++) {
        if (nah_tild[i] == '$' && nah_tild[i + 1] != '(' 
            && nah_tild[i + 1] != ' '  && nah_tild[i + 1] != '\0') {
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