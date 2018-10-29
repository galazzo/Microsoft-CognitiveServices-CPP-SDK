#include <iostream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>

#include "microsoft/utils/http.h"
#include "microsoft/cognitive/cv/cv.h"

using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision;

void print_usage() 
{
	printf("Usage: face -i images/sample08.jpg | http://www.comstoso.com/sample.jpg -s=visionKey\n");
}

int main(int argc, char **argv)
{
	std::cout << "Microsoft Cognitive Services in C++" << endl;
	std::cout << "Custom Vision - Face" << endl;

	std::string subscriptionKeyFilePath = "visionKey";	
	std::string input;
		
	//Specifying the expected options
	static struct option long_options[] = {
		{"subscriptionKey",      optional_argument,       0,  's' },
		{"image",   required_argument, 0,  'i' },		
		{0,           0,                 0,  0   }
	};

	int opt = 0;
	int long_index = 0;
	while ((opt = getopt_long_only(argc, argv, "", long_options, &long_index)) != -1)
	{
		cout << "opt: " << opt << " optarg: " << optarg << endl;

		switch (opt) {
		case 's': subscriptionKeyFilePath = optarg;
			break;
		case 'i': input = optarg;
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

	Json::Value options;

	options["returnFaceId"] = "true";
	options["returnFaceLandmarks"] = "true";

	if (isUrl(input))
	{
		cout << "analyzing url..." << endl;
		Face::Detect(input, options, ApiServerRegion::West_Europe, subscriptionKey);
		//id.debug();
	} else 
	{
		cout << "analyzing image..." << endl;
		HttpContent wt;

		std::ifstream inputfs;
		inputfs.open(input, std::ios::binary);

		// copies all data into buffer
		std::vector<char> buffer((std::istreambuf_iterator<char>(inputfs)), (std::istreambuf_iterator<char>()));

		wt.size = buffer.size();
		wt.buffer = reinterpret_cast<char*>(buffer.data());

		Face::Detect(&wt, options, ApiServerRegion::West_Europe, subscriptionKey);
		//id.debug();
	}

	return 0;
}
