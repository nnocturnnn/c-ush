#include "ush.h"

char **get(char *n) {
    const char h = ' ';
    char **s = (char **)malloc(sizeof(char *) * (mx_count_words(n, h)) + 5);
    int counter = 0;

    for (int i = 0; i < mx_strlen(n); i++) {
        if ((n[i] == '$' && n[i - 1] != '\\') 
             && n[i + 1] != '(' && n[i + 1] != ' '  && n[i + 1] != '\0') {
            int z[2];
            z[0] = i;
            while (n[i] != ' ' && i < mx_strlen(n) 
                   && n[i - 1] != '}')
                i++;
            z[1] = i;
            s[counter++] = mx_strndup(n + z[0], z[1] - z[0]);
        }
    }
    s[counter] = NULL;
    return s;
}

char **getslesh(char *n) {
    const char h = ' ';
    char **s = (char **)malloc(sizeof(char *) * (mx_count_words(n, h)) + 5);
    int counter = 0;

    for (int i = 0; i < mx_strlen(n); i++) {
        if ((n[i] == '$' && n[i - 1] == '\\') && n[i + 1] != '(' 
             && n[i + 1] != ' '  && n[i + 1] != '\0') {
            int z[2];
            z[0] = i - 1;
            while (n[i] != ' ' && i < mx_strlen(n) 
                   && n[i - 1] != '}')
                i++;
            z[1] = i;
            s[counter++] = mx_strndup(n + z[0], z[1] - z[0]);
        }
    }
    s[counter] = NULL;
    return s;
}

char *rec_cord(char *str, char *word1, char *word2, int coord) {
    char *newstr = mx_strnew(mx_strlen(str) - mx_strlen(word1)
                             + mx_strlen(word2));
    int as = 0;
    int g = -1;

    for (int h = 0; str[h]  != '\0'; h++) {
        if (h != coord)
            newstr[as++] = str[h];
        if (h == coord){
            while (word2[++g] != '\0')
                newstr[as + g] = word2[g];
            as += g;
            h += strlen(word1);
        }
    }
    return newstr;
}

char *replaceslesh(char *n) {
    char **aa = getslesh(n);
    for (int i = 0; aa[i] != NULL; i++)
        n = mx_replace_substr(n, aa[i], ++aa[i]);
    return n;
}

char *mx_replace_dolars(char *n, char ***env, char **s) {
    char **a = get(n);
    int z[3];
    for (z[0] = 0; n[z[0]] != '\0'; z[0]++)
        if (n[z[0]] == '$' && n[z[0] - 1] != '\\')
            for (z[1] = 0; a[z[1]] != NULL; z[1]++) { 
                z[2] = mx_strlen(a[z[1]]);
                if (strncmp(n + z[0], a[z[1]], z[2]) == 0) {
                    if (a[z[1]][1] != '{')
                        n = repwithoutskob(n, env, s, z);
                    else if (a[z[1]][1] == '{')
                        n = repwithskob(n, env, s, z);
                }
            }
    return replaceslesh(n);
}
