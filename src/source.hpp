#include <string>
#include <fstream>

bool format(const std::string filename, const std::string format){
    for(size_t i = 0; i <= filename.length() && i <= format.length(); ++i){
        if(filename[filename.length() - i] != format[format.length() - i]){
            return false;
        }
    }
    return true;
}

std::string getSource(const std::string filename){
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return "";
    }

    std::string content;
    std::string line;
    while (std::getline(inputFile, line)) {
        content.append(line);
        content += '\n';
    }
    inputFile.close();
    return content;

}  