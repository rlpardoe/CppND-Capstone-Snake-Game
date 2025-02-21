#ifndef UTILS_H
#define UTILS_H
#include "utils.h"
#include <stdexcept>
#include <fstream>
#include <sstream>

SetupInfo parseSetupInfo(std::string filepath){
    std::ifstream inputFile(filepath);
    if (!inputFile.is_open()){
        throw std::runtime_error("Failed to open file at provided path");
    }
    std::string line;
    std::string key;
    unsigned int value;
    SetupInfo ret;
    while (std::getline(inputFile, line)){
        std::istringstream stringStream(line);
        stringStream >> key >> value;
        
        if (key.compare("ScreenDim:") == 0) ret.screenDim = value;
        if (key.compare("GridDim:") == 0) ret.gridDim = value;
        if (key.compare("NumBarriers:") == 0) ret.numBarr = value;
    }

    return ret;
}

#endif