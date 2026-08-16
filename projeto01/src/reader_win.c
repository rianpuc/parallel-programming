/* Windows implementation of list_dir */
#ifdef _WIN32
#include "reader.h"
#include <windows.h>

char file_names[MAX_FILES][MAX_PATH];
int file_count = 0;

void list_dir_win(char *dirpath) {
    printf("Listing all datasets available: \n");
    file_count = 0;
    size_t len = strlen(dirpath);
    if (len + 2 >= MAX_PATH) return;
    if (dirpath[len-1] == '\\' || dirpath[len-1] == '/') {
        strcat(dirpath, "*");
    } else {
        strcat(dirpath, "\\*");
    }

    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA(dirpath, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0) continue;
        char full[MAX_PATH];
        strncpy(full, dirpath, MAX_PATH-1);
        full[MAX_PATH-1] = '\0';
        size_t fl = strlen(full);
        if (fl >= 2 && full[fl-1] == '*' ) {
            full[fl-1] = '\0';
            if (full[fl-2] == '\\') full[fl-2] = '\0';
        }
        if (strlen(full) + 1 + strlen(ffd.cFileName) < MAX_PATH) {
            if (!(full[strlen(full)-1] == '\\' || full[strlen(full)-1] == '/')) strcat(full, "\\");
            strcat(full, ffd.cFileName);
            char abs[MAX_PATH];
            DWORD rr = GetFullPathNameA(full, MAX_PATH, abs, NULL);
            if (rr != 0 && rr < MAX_PATH) {
                if (file_count < MAX_FILES) {
                    strncpy(file_names[file_count], abs, MAX_PATH-1);
                    file_names[file_count][MAX_PATH-1] = '\0';
                    printf("%d. %s\n", file_count, ffd.cFileName);
                    file_count++;
                }
            }
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    FindClose(hFind);
    printf("Select a dataset > ");
}
#endif
