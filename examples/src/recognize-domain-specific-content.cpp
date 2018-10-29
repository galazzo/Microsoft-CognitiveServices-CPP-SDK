#include <iostream>
#include <iterator>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>

#include "microsoft/cognitive/cognitive.h"
#include "microsoft/cognitive/cv/domainspecificcontent.h"

using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision;

void print_usage() {
    printf("Usage: recognize-domain-specific-content -model landmarks|celebrities -i images/sample06.jpg\n");
}

int main(int argc, char **argv)
{
    cout << "Microsoft Cognitive Services in C++" << endl;
    cout << "Recognize Domain Specific Content" << endl;

    std::string subscriptionKeyFilePath = "subscriptionKey";
    std::string model;
    std::string input;

    //Specifying the expected options
    static struct option long_options[] = {
        {"subscriptionKey",      optional_argument,       0,  's' },
        {"model",    required_argument, 0,  'm' },
        {"image",   required_argument, 0,  'i' },
        {0,           0,                 0,  0   }
    };

    int opt= 0;
    int long_index =0;
    while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1)
    {
        switch (opt) {
             case 's' : subscriptionKeyFilePath = optarg;
                 break;
             case 'm' : model = optarg;
                 break;
             case 'i' : input = optarg;
                 break;
             default: print_usage();
                 exit(EXIT_FAILURE);
        }
    }

    std::string subscriptionKey;
    std::ifstream subscriptionKeyFile;
    subscriptionKeyFile.open(subscriptionKeyFilePath);
    std::getline(subscriptionKeyFile, subscriptionKey);
    subscriptionKeyFile.close();

	std::vector<Microsoft::CognitiveServices::ComputerVision::DomainModel> models = Microsoft::CognitiveServices::ComputerVision::DomainSpecificModels(ApiServerRegion::West_Europe, subscriptionKey, "application/octet-stream");
	for (std::vector<Microsoft::CognitiveServices::ComputerVision::DomainModel>::iterator it = models.begin(); it != models.end(); ++it)
	{
		((Microsoft::CognitiveServices::ComputerVision::DomainModel)(*it)).debug();
	}

	if (isUrl(input))
	{
		DomainSpecificContent result = RecognizeDomainSpecificContent(input, model, ApiServerRegion::West_Europe, subscriptionKey);
		result.debug();
	}
	else
	{
		HttpContent wt;

		std::ifstream inputfs;
		inputfs.open(input, std::ios::binary);

		// copies all data into buffer
		std::vector<char> buffer((std::istreambuf_iterator<char>(inputfs)), (std::istreambuf_iterator<char>()));

		wt.size = buffer.size();
		wt.buffer = reinterpret_cast<char*>(buffer.data());

		DomainSpecificContent result = RecognizeDomainSpecificContent(&wt, model, ApiServerRegion::West_Europe, subscriptionKey);
		result.debug();
	}

    return 0;
}
