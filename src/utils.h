#include <string>

struct windowInfo
{
    unsigned int screenDim;
    unsigned int gridDim;
    unsigned int numBarr;
};

windowInfo parseWindowInfo(std::string filepath);