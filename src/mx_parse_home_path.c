#include "ush.h"

char *mx_parse_home_path(char *path, int reverse, char **env) {
    char *new;
    char *home_path = mx_get_env_var("HOME", env);
    if(!path)
        return NULL;
    if(mx_get_substr_index(path, reverse ? "~" : home_path) < -1)
        return mx_strdup(path);
    if (reverse)
        new = mx_pathjoin(home_path, path + 1);
    else {
        if (*(path + mx_strlen(home_path)) == '\0')
            new = mx_strdup("~");
        else
            new = mx_pathjoin("~", path + mx_strlen(home_path));
    }
    return new;
}