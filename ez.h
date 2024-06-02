// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BLUE "\x1b[34m"
#define RED "\x1b[31m"
#define INFO(message, args...) printf("\x1b[1m[%s%s\x1b[0;1m]\x1b[0m " message"\n", BLUE, "INFO", ##args);
#define ERROR(message, args...) printf("\x1b[1m[%s%s\x1b[0;1m]\x1b[0m " message"\n", RED, "ERROR", ##args);

#define MAX_THREADS 16
pthread_t thread_pool[MAX_THREADS] = { 0 };

int fail = 0;
pthread_mutex_t fail_mutex;

typedef struct {
    char **args;
    char *message;
    size_t length;
} Command;

Command *cmd_new(void) {
    Command *cmd = malloc(sizeof(Command));
    memset(cmd, 0, sizeof(Command));
    return cmd;
}

void cmd_push(Command *cmd, char *arg) {
    if (cmd->args == 0) {
        cmd->args = malloc(2 * sizeof(char*));
    } else {
        cmd->args = realloc(cmd->args, (cmd->length + 2) * sizeof(char*));
    }
    size_t len = strlen(arg) + 1;
    cmd->args[cmd->length] = malloc(len);
    strncpy(cmd->args[cmd->length], arg, len);
    cmd->length += 1;
    cmd->args[cmd->length] = 0;
}

void cmd_message(Command *cmd, char *message) {
    size_t len = strlen(message) + 1;
    cmd->message = malloc(len);
    strncpy(cmd->message, message, len);
}

void cmd_free(Command *cmd) {
    for (size_t i = 0; i < cmd->length; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
    if (cmd->message) {
        free(cmd->message);
    }
    free(cmd);
}

int cmd_execute(Command *cmd) {
    int status;

    pid_t pid = fork();
    if (pid == 0) {
        execvp(cmd->args[0], cmd->args);
        _exit(EXIT_FAILURE);
    }

    waitpid(pid, &status, 0);
    return status;
}

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Command **cmds;
    size_t size;
} CommandQueue;

CommandQueue global_queue = { 0 };

void queue_push(CommandQueue *queue, Command *cmd) {
    pthread_mutex_lock(&queue->mutex);
    queue->cmds = realloc(queue->cmds, (queue->size + 1) * sizeof(Command*));
    queue->cmds[queue->size] = cmd;
    queue->size += 1;
    pthread_mutex_unlock(&queue->mutex);
    pthread_cond_signal(&queue->cond);
}

Command *queue_pop(CommandQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->size == 0) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }
    Command *cmd = queue->cmds[queue->size - 1];
    queue->size -= 1;
    pthread_mutex_unlock(&queue->mutex);
    return cmd;
}

void *queue_worker(void*) {
    while (!fail) {
        Command *cmd = queue_pop(&global_queue);
        if (cmd == 0) {
            break;
        }
        INFO("%s", cmd->message);
        int status = cmd_execute(cmd);
        if (status) {
            pthread_mutex_lock(&fail_mutex);
            fail = 1;
            pthread_mutex_unlock(&fail_mutex);
        }
        cmd_free(cmd);
    }
    return 0;
}

void queue_shutdown(void) {
    for (int i = 0; i < MAX_THREADS; i++) {
        queue_push(&global_queue, 0);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(thread_pool[i], 0);
    }
}

void ez_mkdir(char *path) {
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

#define assert(expr) if (expr != 0) return -1;