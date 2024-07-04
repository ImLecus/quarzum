#pragma once
#include "cli.h"

using namespace Quarzum::Debug;
namespace Quarzum::CLI {
    string outputFileName = "output";
    string inputFileName = "";
    std::unordered_map<string, uint8> flags = {
        {"--a", 1},
        {"--architecture", 1},
        {"--d", 2},
        {"--debug", 2},
        {"--o", 3},
        {"--output", 3}
    };

    string commands[] = {"run", "install", "create"};

    void parseCLIArgs(const char** argv){
        string filename;
        for(int i = 1; argv[i]; ++i){

            string arg = argv[i];
            auto it = flags.find(arg);
            if(it != flags.end()){
                switch (it->second)
                {
                case 1:

                    break;
                case 2:
                    debugMode = true;
                    break;

                case 3:
                    outputFileName = arg;
                    ++i;
                    break;
                
                default:
                    err("No such argument '" + arg +"'.");
                    break;
                }
            }
            else {
                inputFileName = arg;
            }
        }
    }
}