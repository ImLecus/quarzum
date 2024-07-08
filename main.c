#include <stdio.h>
#include "include/quarzum.h"

int main(int argc, char** argv) {
    if(argc < 2){
        return 1;
    }
    
    compile(argv[1]);
    return 0;
}