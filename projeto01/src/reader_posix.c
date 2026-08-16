#ifndef _WIN32
#include "reader.h"
#include <dirent.h>

char file_names[MAX_FILES][MAX_PATH];
int file_count = 0;

void list_dir_posix(char *dirpath){
    printf("Listing all datasets available: \n");
    DIR* dirFile = opendir(dirpath);
    struct dirent* file;
    if (dirFile) {
        while ((file = readdir(dirFile)) != NULL) {
            if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
                continue;
            }
            char path[MAX_PATH];
            snprintf(path, sizeof(path), "%s/%s", dirpath, file->d_name);
            char *actual_path = realpath(path, NULL);
            if (actual_path != NULL) {
                if (file_count < MAX_FILES) {
                    strncpy(file_names[file_count], actual_path, MAX_PATH - 1);
                    file_names[file_count][MAX_PATH - 1] = '\0';
                    printf("%d. %s\n", file_count, file->d_name);
                    file_count++;
                }
                free(actual_path);
            }
        }
        closedir(dirFile);
    }
    printf("Select a dataset > ");
}

#endif
