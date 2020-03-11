#include "ush.h"

static char **getvars(char *n) {
    const char h = ' ';
    char **vars = (char **)malloc(sizeof(char *) * (mx_count_words(n, h)) + 5);
    int counter = 0;
    
    for (int i = 0; i < mx_strlen(n); i++) {
        if ((n[i] == '$' && n[i - 1] != '\\') 
             && n[i + 1] != '(' && n[i + 1] != ' '  && n[i + 1] != '\0') {
            int arr[2];
            
            arr[0] = i;
            while (n[i] != ' ' && i < mx_strlen(n) 
                   && n[i - 1] != '}')
                i++;
            arr[1] = i;
            vars[counter++] = mx_strndup(n + arr[0], arr[1] - arr[0]);
        }
    }
    vars[counter] = NULL;
    return vars;
}


static char **getsleshvars(char *n) {
    const char h = ' ';
    char **vars = (char **)malloc(sizeof(char *) * (mx_count_words(n, h)) + 5);
    int counter = 0;
    
    for (int i = 0; i < mx_strlen(n); i++) {
        if ((n[i] == '$' && n[i - 1] == '\\') && n[i + 1] != '(' 
             && n[i + 1] != ' '  && n[i + 1] != '\0') {
            int arr[2];

            arr[0] = i - 1;
            while (n[i] != ' ' && i < mx_strlen(n) 
                   && n[i - 1] != '}')
                i++;
            arr[1] = i;
            vars[counter++] = mx_strndup(n + arr[0], arr[1] - arr[0]);
        }
    }
    vars[counter] = NULL;
    return vars;
}


static char *rec_cord(char *str, char *word1, char *word2, int coord) {
    char *newstr = mx_strnew(mx_strlen(str) - mx_strlen(word1)
                             + mx_strlen(word2));
    int as = 0;
    int gg = -1;

    for (int h = 0; str[h]  != '\0'; h++) {
        if (h != coord)
            newstr[as++] = str[h];
        if (h == coord){
            while (word2[++gg] != '\0')
                newstr[as + gg] = word2[gg];
            as += gg;
            h += strlen(word1);
        }
    }
    return newstr;
}

char *mx_replace_dolars(char *n, char ***env, char **vars) {
    char **a = getvars(n); 
    char **aa = getsleshvars(n);

    (void)vars;
    for (int var = 0; n[var] != '\0'; var++) {
        if (n[var] == '$' && n[var - 1] != '\\') { 
            for (int gg = 0; a[gg] != NULL; gg++) { 
                int lenvar = mx_strlen(a[gg]);
                
                if (strncmp(n + var, a[gg], lenvar) == 0) {
                    if (a[gg][1] != '{') {
                    if (mx_get_env_var(strndup(n + var, lenvar) + 1, *(env)) != NULL)
                        n = rec_cord(n, strndup(n + var, lenvar - 1), mx_get_env_var(strndup(n + var, lenvar) + 1, *(env)), var);
                    else
                        n = rec_cord(n, strndup(n + var, lenvar), "", var);
                    }
                    else
                    if (mx_get_env_var(strndup(n + var, lenvar - 1) + 2, *(env)) != NULL)
                        n = rec_cord(n, strndup(n + var, lenvar - 1), mx_get_env_var(strndup(n + var, lenvar - 1) + 2, *(env)), var);
                    else
                        n = rec_cord(n, strndup(n + var, lenvar - 1), "", var);
                }
            }
        }
    }
    for (int i = 0; aa[i] != NULL; i++)
                n = mx_replace_substr(n, aa[i], ++aa[i]);
    return n;
}
