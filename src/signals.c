#include "ush.h"

void proc_signal_handler(int signo) {
	if (signo == SIGINT) {
		signal(SIGINT, proc_signal_handler);
	}
}

void signal_handler(int signo) {
    extern char **environ;

	if (signo == SIGINT) {
        mx_printstr("\n");
        mx_display(environ);
		signal(SIGINT, signal_handler);
	} else if (signo == EOF) {
        mx_printstr("\n");
		signal(EOF, signal_handler);
    } else if (signo == SIGTSTP) {
        signal(SIGTSTP, signal_handler);
    }
}
