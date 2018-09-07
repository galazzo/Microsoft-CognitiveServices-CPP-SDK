#include <iostream>
#include <string.h>

#include <fstream>      // std::ifstream

#include "microsoft/cognitive/cv/recognize_text.h"

using namespace Microsoft::CognitiveServices::ComputerVision;

int main(int argc, char **argv)
{
    cout << "Microsoft Cognitive Services in C++" << endl;
	cout << "Recognize Text" << endl;

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

    Text::RecognitionResult result = Text::RecognizeText(&wt, subscriptionKey, "application/octet-stream");
    result.debug();

    return 0;
}
