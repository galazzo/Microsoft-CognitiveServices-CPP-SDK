#include "microsoft/cognitive/cognitive.h"

void Microsoft::CognitiveServices::ToFile(std::string filename, ios_base::openmode mode, const char* buffer, int size) {

    ofstream fout;

    fout.open(filename, mode);

    fout.write((char*) buffer, size);

    fout.close();
}

std::string Microsoft::CognitiveServices::ApiServer(ApiServerRegion region)
{
    std::string result;

    switch(region) {
        case ApiServerRegion::West_US: result ="westus.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::West_US_2: result ="westus2.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::East_US: result ="eastus.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::East_US_2: result ="eastus2.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::West_Central_US: result ="westcentralus.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::South_Central_US: result ="southcentralus.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::West_Europe: result ="westeurope.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::North_Europe: result ="northeurope.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::Southeast_Asia: result ="southeastasia.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::East_Asia: result ="eastasia.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::Australia_East: result ="australiaeast.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::Brazil_South: result ="brazilsouth.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::Canada_Central: result ="canadacentral.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::Central_India: result ="centralindia.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::UK_South: result ="uksouth.api.cognitive.microsoft.com"; break;
        case ApiServerRegion::Japan_East: result ="japaneast.api.cognitive.microsoft.com"; break;
        default : break;
    }

    return result;
}
