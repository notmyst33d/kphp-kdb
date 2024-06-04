//usr/bin/cc -x c -g -O2 -Wall -lpthread -o .buildbin "$0" && ./.buildbin; exit

#include "ez.h"

#include <dirent.h>

char *cxx_compile_flags[] = { "-g", "-std=gnu++17", "-IKPHP", "-Icommon", "-Idrinkless", 0 };
char *main_flags[] = { "-Lout", "-L/usr/lib/openssl-1.1", "-lcrypto", "-lz", "-lre2", "-lpcre", "-lkdb-common", "-lkdb-net", "-lkdb-crypto", "-lkdb-vv", "-lkdb-binlog", "-lkdb-kfs", "-ldrinkless", 0 };

#define cxx(in, out) global_queue_compile(in, out, cxx_compile_flags, COMPILER_CXX)
#define ld global_queue_link

typedef struct {
    char *input;
    char *output;
} File;

typedef struct {
    File **files;
    size_t length;
} Files;

File *file_new(char *input, char *output) {
    File *file = malloc(sizeof(File));
    size_t input_len = strlen(input) + 1;
    size_t output_len = strlen(output) + 1;
    file->input = malloc(input_len);
    file->output = malloc(output_len);
    strncpy(file->input, input, input_len);
    strncpy(file->output, output, output_len);
    return file;
}

void file_free(File *file) {
    free(file->input);
    free(file->output);
    free(file);
}

void files_push(Files *files, File *file) {
    files->files = realloc(files->files, (files->length + 1) * sizeof(File*));
    files->files[files->length++] = file;
}

void files_free(Files *files) {
    for (size_t i = 0; i < files->length; i++) {
        free(files->files[i]->input);
        free(files->files[i]->output);
        free(files->files[i]);
    }
    free(files->files);
    files->length = 0;
}

void build_kphp_program(void) {
    Command *cmd = cmd_new();
    cmd_message(cmd, "kphp2cpp main.php");
    cmd_push(cmd, "out/kphp2cpp");
    cmd_push(cmd, "-f");
    cmd_push(cmd, "KPHP/functions.txt");
    cmd_push(cmd, "-d");
    cmd_push(cmd, "out/kphp_program");
    cmd_push(cmd, "kphp_program/main.php");
    global_queue_push(cmd);
}

void find_kphp_output(Files *files) {
    struct dirent *entry;
    DIR *dir = opendir("out/kphp_program/kphp");
    while ((entry = readdir(dir))) {
        size_t len = strlen(entry->d_name) + 1;
        if (strcmp(entry->d_name + len - 5, ".cpp") == 0) {
            char input_path[512] = { 0 }, output_path[512] = { 0 };
            sprintf(input_path, "out/kphp_program/kphp/%s", entry->d_name);
            sprintf(output_path, "out/kphp_program/objs/%s", entry->d_name);
            size_t output_len = strlen(output_path) + 1;
            output_path[output_len - 4] = 'o';
            output_path[output_len - 3] = 0;
            files_push(files, file_new(input_path, output_path));
        }
    }
    closedir(dir);
}

void build_kphp_output(Files *files) {
    for (int i = 0; i < files->length; i++) {
        cxx(files->files[i]->input, files->files[i]->output);
    }
}

char **link_kphp_output(Files *files) {
    char **objs = malloc((files->length + 2) * sizeof(char*));
    memset(objs, 0, (files->length + 2) * sizeof(char*));
    objs[0] = "out/kphp-engine.o";
    for (int i = 1; i < files->length; i++) {
        objs[i] = files->files[i]->output;
    }
    ld("out/kphp_program/main", objs, main_flags, COMPILER_CXX);
    return objs;
}

int main(void) {
    ez_mkdir("out/kphp_program");
    ez_mkdir("out/kphp_program/objs");

    global_queue_start();

    build_kphp_program();
    assert(global_queue_wait());

    Files files = { 0 };
    find_kphp_output(&files);
    build_kphp_output(&files);
    assert(global_queue_wait());

    char **d = link_kphp_output(&files);
    assert(global_queue_wait());
    free(d);
    files_free(&files);

    global_queue_shutdown();

    INFO("Done");
    return 0;
}
