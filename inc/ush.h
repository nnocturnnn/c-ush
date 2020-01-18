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
#include "libmx/inc/libmx.h"

char **init_envr(int argc, char **argv, char **envr);
void mx_display(void);
char *parse_home_path(char *path, int reverse);
int ft_isemptystr(char *str, int consider_space);
void mx_get_input(char **input);

#endif 
