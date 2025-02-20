#include <string>

struct windowInfo
{
    unsigned int screenDim;
    unsigned int gridDim;
};

windowInfo parseWindowInfo(std::string filepath);