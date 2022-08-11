#include "stdio.h"

#define LS 5863588
#define CD 5863276
#define MKDIR 210720772860
#define PWD 193502992

const unsigned long hash(const char *str) {
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

int main(int argc, char *argv[]) {
    char* this = argv[1];
    char *p_command = this;
    unsigned long newHash = hash(p_command);
    printf("hash: %d\n", newHash);
    switch(newHash) {
    case LS:
        printf("Running ls...\n");
        break;
    case CD:
        printf("Running cd...\n");
        break;
    case MKDIR:
        printf("Running mkdir...\n");
        break;
    case PWD:
        printf("Running pwd...\n");
        break;
    default:
        printf("[ERROR] '%s' is not a valid command. %d\n", p_command, newHash);
    }
}