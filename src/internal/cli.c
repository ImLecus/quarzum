/*
 * Quarzum Compiler - cli.c
 * Version 1.0, 09/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../../include/quarzum/cli.h"

void printUsage(){
    printf("Usage: quarzum <file> <flags>\n");
    printf("Options:\n");
    printf("  -h, --help            Show this message.\n");
    printf("  -v, --version         Show Quarzum Compiler version\n");
    printf("  -a, --arch <name>     Set the target architecture.\n");
    printf("  -d, --debug           Run in debug mode.\n");
    printf("  -o, --output <name>   Set the output file name.\n");
    printf("  --no-warnings         Disable warning messages.\n");
    printf("  --show_tokens         Show the program tokens while compiling.\n");
}

void CLI(int argc, char** argv){
    if(argc < 2){
        printUsage();
        return;
    }
    // if flag
    if(argc >= 2 && argv[1][0] == '-'){
        int search = binarySearch(argv[1],flagNames,FLAG_NUMBER);
        switch (search)
        {
        case -1:
            printUsage();
            break;
        case 0:
        case 1:
            printUsage();
            break;
        case 2:
        case 3:
            flags |= debugMode;
            break;
        case 4:
        case 5:
            printf("Version 1.0\n");
            break;
        case 6:
        case 7:
            // arch
            break;
        case 8:
        case 9:
            outputFileName = argv[1];
            break;
        case 10:
            flags |= noWarningMessages;
            break;
        case 11:
            flags |= showTokens;
            break;
        default:
            break;
        }
    }
    else {
        compile(argv[1]);
    }
    
}