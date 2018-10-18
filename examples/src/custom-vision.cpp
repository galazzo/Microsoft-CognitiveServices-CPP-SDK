#include <iostream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>

#include "microsoft/cognitive/cv/custom_vision_prediction.h"

using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision::CustomVision;

void print_usage() {
	printf("Usage: custom-vision -source url|image -i images/sample06.jpg|http://www.comstoso.com/sample.jpg -projectid <YOUR PROJECT ID>\n");
}

int main(int argc, char **argv)
{
	std::cout << "Microsoft Cognitive Services in C++" << endl;
	std::cout << "Custom Vision - Predict" << endl;

	std::string predictionKeyFilePath = "predictionKey";
	std::string source;
	std::string input;
	std::string projectid;

	//Specifying the expected options
	static struct option long_options[] = {
		{"predictionKey",      optional_argument,       0,  'k' },
		{"source",    required_argument, 0,  's' },
		{"image",   required_argument, 0,  'i' },
		{"projectid",   required_argument, 0,  'p' },
		{0,           0,                 0,  0   }
	};

	int opt = 0;
	int long_index = 0;
	while ((opt = getopt_long_only(argc, argv, "", long_options, &long_index)) != -1)
	{
		switch (opt) {
		case 'k': predictionKeyFilePath = optarg;
			break;
		case 's': source = optarg;
			break;
		case 'i': input = optarg;
			break;
		case 'p': projectid = optarg;
			break;
		default: print_usage();
			exit(EXIT_FAILURE);
		}
	}

	std::string predictionKey;
	std::ifstream predictionKeyFile;
	predictionKeyFile.open(predictionKeyFilePath);
	std::getline(predictionKeyFile, predictionKey);
	predictionKeyFile.close();

	if (source == "url") 
	{
		Prediction id = PredictImageUrlWithNoStore(input, projectid, ApiServerRegion::South_Central_US, predictionKey);
		id.debug();
	} 

	if (source == "image") 
	{
		HttpContent wt;

		std::ifstream inputfs;
		inputfs.open(input, std::ios::binary);

		// copies all data into buffer
		std::vector<char> buffer((std::istreambuf_iterator<char>(inputfs)), (std::istreambuf_iterator<char>()));

		wt.size = buffer.size();
		wt.buffer = reinterpret_cast<char*>(buffer.data());

		Prediction id = PredictImageWithNoStore(&wt, projectid, ApiServerRegion::South_Central_US, predictionKey);
		id.debug();
	}

	return 0;
}
