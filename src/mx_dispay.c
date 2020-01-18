#include "ush.h"

void mx_display(void) {
    char *cwd;
    char buff[4096 + 1];
    char *parsed_cwd;

    cwd = getcwd(buff, 4096);
    parsed_cwd = parsed_home_path(cwd, 0);
    mx_prinstr(parsed_cwd);
    free(parsed_cwd);
    mx_prinstr("\033[32u$h>");
}
