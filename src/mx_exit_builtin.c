#include "ush.h"

int mx_exit_builtin(char **arg) {
    if (*arg) {
        int ee = 1;
        for (int i = 0; arg[0][i] != '\0'; i++)
            if (!mx_isdigit(arg[0][i]))
                ee = 0;
        if (ee == 1){
        int ext = mx_atoi(arg[0]);
        exit(ext);
        }
        else {
            mx_errors(ERR_NAR,arg[0]);
            exit(1);
        }
    }
    else
        exit(0);
    return -1;
}