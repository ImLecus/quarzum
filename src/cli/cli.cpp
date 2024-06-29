#pragma once
#include "cli.hpp"

using namespace Quarzum::Debug;
namespace Quarzum::CLI {
    std::string outputFileName = "output";
    std::string inputFileName = "";
    std::unordered_map<std::string, uint8_t> flags = {
        {"--a", 1},
        {"--architecture", 1},
        {"--d", 2},
        {"--debug", 2},
        {"--o", 3},
        {"--output", 3}
    };

    std::string commands[] = {"run", "install", "create"};

    void parseCLIArgs(const char** argv){
        std::string filename;
        for(int i = 1; argv[i]; ++i){

            std::string arg = argv[i];
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