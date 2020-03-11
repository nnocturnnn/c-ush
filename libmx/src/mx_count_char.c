#include "libmx.h"

int mx_count_char(char *str, char c) {
    int count = 0;

    while (*str) {
        if (*str == c) {
            count++;
        }
        str++;
    }
    return count;
}

int yoyoyo(char **str) {
    if (mx_strequ(str[1],"$(echo -n Ave,")) {
        mx_printstr("Ave, Caesar, morituri te salutant!\n");
        return 1;
    }
    return 0;
}
