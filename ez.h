// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>

#ifndef CC
#define CC "cc"
#endif

#ifndef THREADS
#define THREADS 16
#endif

#define BLUE "\x1b[34m"
#define RED "\x1b[31m"
#define INFO(message, args...) printf("\x1b[1m[%s%s\x1b[0;1m]\x1b[0m " message"\n", BLUE, "INFO", ##args);
#define ERROR(message, args...) printf("\x1b[1m[%s%s\x1b[0;1m]\x1b[0m " message"\n", RED, "ERROR", ##args);
#define assert(expr) if (expr != 0) return -1

void ez_mkdir(char *path) {
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

pthread_t global_thread_pool[THREADS] = { 0 };

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
    int working_threads;
    pthread_mutex_t working_threads_mutex;
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

void *queue_worker(void *queue_ptr) {
    CommandQueue *queue = queue_ptr;
    while (!fail) {
        Command *cmd = queue_pop(queue);
        if (cmd == 0) {
            break;
        }
        pthread_mutex_lock(&queue->working_threads_mutex);
        queue->working_threads++;
        pthread_mutex_unlock(&queue->working_threads_mutex);
        INFO("%s", cmd->message);
        int status = cmd_execute(cmd);
        if (status) {
            pthread_mutex_lock(&fail_mutex);
            fail = 1;
            pthread_mutex_unlock(&fail_mutex);
        }
        cmd_free(cmd);
        pthread_mutex_lock(&queue->working_threads_mutex);
        queue->working_threads--;
        pthread_mutex_unlock(&queue->working_threads_mutex);
    }
    return 0;
}

void queue_shutdown(CommandQueue *queue, pthread_t *thread_pool, int thread_count) {
    for (int i = 0; i < thread_count; i++) {
        queue_push(queue, 0);
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_pool[i], 0);
    }
}

int queue_wait(CommandQueue *queue, pthread_t *thread_pool, int thread_count) {
    while ((queue->size > 0 || queue->working_threads > 0) && !fail) {
        usleep(1000);
    }
    if (fail) {
        ERROR("Compilation aborted");
        queue_shutdown(queue, thread_pool, thread_count);
        return -1;
    }
    return 0;
}

void queue_start(CommandQueue *queue, pthread_t *thread_pool, int thread_count) {
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_pool[i], 0, queue_worker, queue);
    }
}

#define global_queue_push(cmd) queue_push(&global_queue, cmd)
#define global_queue_start() queue_start(&global_queue, global_thread_pool, THREADS)
#define global_queue_wait() queue_wait(&global_queue, global_thread_pool, THREADS)
#define global_queue_shutdown() queue_shutdown(&global_queue, global_thread_pool, THREADS)

void queue_cc(CommandQueue *queue, char *in, char *out, char **flags) {
    Command *cmd = cmd_new();
    char message[512];

    sprintf(message, CC " %s", in);
    cmd_message(cmd, message);

    cmd_push(cmd, CC);
    for (int i = 0; flags[i] != 0; i++) {
        cmd_push(cmd, flags[i]);
    }
    cmd_push(cmd, "-c");
    cmd_push(cmd, "-o");
    cmd_push(cmd, out);
    cmd_push(cmd, in);

    queue_push(queue, cmd);
}

void queue_ld(CommandQueue *queue, char *out, char **objs, char **flags) {
    Command *cmd = cmd_new();
    char message[512];

#ifdef USE_LD
    sprintf(message, LD " %s", out);
#else
    sprintf(message, "ld %s", out);
#endif
    cmd_message(cmd, message);

    cmd_push(cmd, CC);
#ifdef USE_LD
    cmd_push(cmd, "-fuse-ld=" LD);
#endif
    for (int i = 0; flags[i] != 0; i++) {
        cmd_push(cmd, flags[i]);
    }
    cmd_push(cmd, "-o");
    cmd_push(cmd, out);
    for (int i = 0; objs[i] != 0; i++) {
        cmd_push(cmd, objs[i]);
    }

    queue_push(queue, cmd);
}

void global_queue_cc(char *in, char *out, char **flags) {
    queue_cc(&global_queue, in, out, flags);
}

void global_queue_ld(char *out, char **objs, char **flags) {
    queue_ld(&global_queue, out, objs, flags);
}
