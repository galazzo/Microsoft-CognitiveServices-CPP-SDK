#include <iostream>
#include <string.h>
#include <fstream>

#include "microsoft/cognitive/cognitive.h"
#include "microsoft/cognitive/cv/analysis.h"

using namespace Microsoft::CognitiveServices;

int main(int argc, char **argv)
{
    cout << "Microsoft Cognitive Services in C++" << endl;
    cout << "Tag" << endl;

    std::ifstream subscriptionKeyFile;
    std::string subscriptionKey;
    subscriptionKeyFile.open("subscriptionKey");
    std::getline(subscriptionKeyFile, subscriptionKey);
    subscriptionKeyFile.close();

    HttpContent wt;

    std::ifstream input;
    input.open(argv[1], std::ios::binary );

    // copies all data into buffer
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

    wt.size = buffer.size();
    wt.buffer = reinterpret_cast<char*>(buffer.data());

    Analysis id = Tags(&wt, ApiServerRegion::West_Europe, subscriptionKey, "application/octet-stream");
    id.debug();

    return 0;
}
