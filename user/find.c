#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
find(char *path, char *file) {
    char buf[512];
    int fd;
    struct dirent de;
    struct stat st;
    
    fd = open(path, 0);
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        
        if (de.name[0] != '\0' && de.name[0] != '.') {
            char str[] = "";
            strcat(str, path);
            strcat(str, "/");
            char * s = str + 1;
            strcat(s, de.name);

            if (stat(s, &st) < 0) {
                printf("find: cannot stat %s\n", s);
                close(de.inum);
                return;
            }

            switch (st.type) {
            case T_FILE:
                if (strcmp(de.name, file) == 0) {
                    printf("%s/%s\n", path, de.name);
                }
                break;

            case T_DIR:
                strcpy(buf, path);
                strcat(buf, "/");
                strcat(buf, de.name);
                printf("%s\n", buf);
                find(buf, file);
                break;
            }            
        }
    }


}

int
main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("Too less auguments");
        exit(0);
    }
    find(".", argv[2]);
    exit(0);
}