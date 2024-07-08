#include <stdio.h>
#include "include/quarzum.h"

int main(int argc, char** argv) {
    if(argc < 2){
        return 1;
    }
    printf("Version %s \n", QUARZUM_COMPILER_VERSION);
    Buffer* b = read(argv[1]);

    Process* p = startProcess("Lex phase");
    TokenList* t = tokenize(b);
    //printf("Lex phase finished. %d tokens found.\n", t->size);
    endProcess(p);

    
    for(unsigned int i = 0; i < t->size; ++i){
        printf("%s\n",t->content[i].value);
    }
    return 0;
}