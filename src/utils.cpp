#include "utils.h"
#include <stdexcept>
#include <fstream>
#include <sstream>

windowInfo parseWindowInfo(std::string filepath){
    std::ifstream inputFile(filepath);
    if (!inputFile.is_open()){
        throw std::runtime_error("Failed to open file at provided path");
    }
    std::string line;
    std::string key;
    unsigned int value;
    windowInfo ret;
    while (std::getline(inputFile, line)){
        std::istringstream stringStream(line);
        stringStream >> key >> value;
        
        if (key.compare("ScreenDim:") == 0) ret.screenDim = value;
        if (key.compare("GridDim:") == 0) ret.gridDim = value;
    }

    return ret;
}