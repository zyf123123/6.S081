#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1
#define LENGTH 35

void primes(int *inputpipeline) {
    close(inputpipeline[WRITEEND]);
    uint32 buf = 1;
    if (read(inputpipeline[READEND], &buf, 4) == 0) {
        return;
    }
    printf("prime %d\n", buf);
    if (buf >= LENGTH) {
        return;
    }
    uint32 n = buf;
    int rightpipeline[2];
    pipe(rightpipeline);

    if (fork() == 0) {
        primes(rightpipeline);
    }
    else {
        close(rightpipeline[READEND]);
        while (read(inputpipeline[READEND], &buf, 4) > 0) {
            uint32 t = buf;
            if (t % n != 0) {
                write(rightpipeline[WRITEEND], &t, sizeof(t));
            }
        }
        close(rightpipeline[WRITEEND]);
        wait(0);
    }
    
    return;
}

int
main(int argc, char *argv[]) {
    int inputpipeline[2];
    pipe(inputpipeline);

    for (uint32 i = 2; i <= LENGTH; i++) {
        uint32 t = i;
        write(inputpipeline[WRITEEND],  &t, sizeof(t));
    }
    close(inputpipeline[WRITEEND]);
    primes(inputpipeline);

    
    wait(0);
    exit(0);
}