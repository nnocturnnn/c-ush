#include "ush.h"

void mx_save_history(char *input, t_ush data) {
    int i = 0;

    if (!mx_isemptystr(input, 1))     
        while(data.history[++i]) {
            data.history[i] = mx_strdup(input);
        }
}

void mx_reset_input_mode(void) {
    tcsetattr(0, TCSANOW, &stored_settings);
}
void set_input_mode (void) {
    struct termios new_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= ~(ICANON|ECHO|ISIG);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSAFLUSH, &new_settings);
}