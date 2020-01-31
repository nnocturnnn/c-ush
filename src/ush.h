#ifndef USH_H 
#define USH_H

#include <stdio.h>
#include <signal.h>
#include <wchar.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <malloc/malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/acl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include <time.h>
#include "../libmx/inc/libmx.h"

char **mx_init_envr(int argc, char **argv, char **envr);
void mx_display(char **env);
char *mx_parse_home_path(char *path, int reverse, char **env);
int mx_isemptystr(char *str, int consider_space);
void mx_exit_shell(char **env);
char *mx_get_env_var(char *var, char **env);
char *mx_pathjoin(char *p1, char *p2);
char **mx_get_input(char **input, char **env);
void signal_handler(int signo, char **env);
void proc_signal_handler(int signo); 

#endif 
