#include "ush.h"

static void set_signals(sigset_t *signals, int im) {
    sigfillset(signals);
    sigdelset(signals, SIGINT);
    sigdelset(signals, SIGQUIT);
    sigdelset(signals, SIGCONT);
    sigdelset(signals, SIGCHLD);
    if (im == 1)
        sigdelset(signals, SIGTSTP);
}

t_list *mx_get_last_process(t_list *processes) {
    for (t_list *cur = processes; cur; cur = cur->next) {
        if (!cur->next) {
            return cur;
        }
    }
    return NULL;
}

t_process *mx_create_process(int im) {
    t_process *process = malloc(sizeof(t_process));

    process->gpid = 0;
    process->commands = NULL;
    process->pos = 0;
    process->input_mode = im;
    set_signals(&process->signals, im);
    posix_spawnattr_init(&process->attrs);
    posix_spawnattr_setpgroup(&process->attrs, process->gpid);
    posix_spawnattr_setsigmask(&process->attrs, &process->signals);
    posix_spawnattr_setflags(&process->attrs, POSIX_SPAWN_SETSIGMASK
                             | POSIX_SPAWN_SETPGROUP);
    posix_spawn_file_actions_init(&process->actions);
    posix_spawn_file_actions_adddup2(&process->actions, im, 1);
    return process;
}

void mx_del_process(t_process **process) {
    if (!MX_WIFSTOPPED((*process)->status)) {
        posix_spawnattr_destroy(&(*process)->attrs);
        posix_spawn_file_actions_destroy(&(*process)->actions);
        mx_strdel(&(*process)->commands);
        free(*process);
        process = NULL;
    }
}

void mx_del_node_list(t_list **list, t_process **d_p) {
    t_list *cur = *list;
    t_list *tmp = NULL;
    t_process *f_p = (t_process*)cur->data;

    if (f_p->pid == (*d_p)->pid) {
        mx_del_process(d_p);
        tmp = *list;
        *list = (*list)->next;
        free(tmp);
        return;
    }
    for (cur = *list; cur; cur = cur->next) {
        f_p = (t_process*)cur->next->data;
        if (f_p->pid == (*d_p)->pid)
            break;
    }
    tmp = cur->next;
    cur->next = tmp->next;
    free(tmp);
    mx_del_process(d_p);
}

void mx_kill_process(void) {
    t_list **processes = mx_get_plist();
    t_process *tmp = NULL;

    for (t_list *cur = *processes; cur; cur = cur->next) {
        tmp = (t_process*)cur->data;
        kill(-tmp->gpid, SIGKILL);
        mx_del_node_list(processes, &tmp);
    }
}

t_list **mx_get_plist(void) {
    static t_list *list = NULL;

    return &list;
}

void mx_continue_process(t_process *process, t_list **all_processes, int fd) {
    if (kill(-process->gpid, SIGCONT)) {
        fprintf(stderr, "fg: %s\n", strerror(errno));
    }
    dprintf(fd, "[%d]    %d continued  %s\n", process->pos, process->pid,
            process->commands);
    if (waitpid(process->gpid, &process->status, WUNTRACED) != -1) {
        if (!MX_WIFSTOPPED(process->status)) {
            mx_del_node_list(all_processes, &process);
        }
        else if (MX_WIFSTOPPED(process->status)) {
            printf("[%d]    %d suspended  %s\n", process->pos, process->pid,
                   process->commands);
        }
    }
}
