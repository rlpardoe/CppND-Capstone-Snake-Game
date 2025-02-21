#include <string>

struct SetupInfo
{
    unsigned int screenDim;
    unsigned int gridDim;
    unsigned int numBarr;
};

SetupInfo parseSetupInfo(std::string filepath);