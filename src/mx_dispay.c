#include "ush.h"

void mx_display(char **env, char **var) {
    char *cwd;
    char buff[4096 + 1];
    char *parsed_cwd;

    cwd = getcwd(buff, 4096);
    parsed_cwd = mx_parse_home_path(cwd, env);
    if (!parsed_cwd ||
        mx_strequ(mx_get_env_var("PROMPT",var),"minimal"))
        mx_printstr("\033[0;32mu$h> \033[0m ");
    else if (mx_strequ(mx_get_env_var("PROMPT",var),"standart")){
        mx_printstr("\033[0;32m");
        mx_printstr(parsed_cwd);
        mx_strdel(&parsed_cwd);
        mx_printstr(" u$h> \033[0m ");
    }
    else {
        mx_printstr(mx_get_env_var("PROMPT",var));
    }
}
