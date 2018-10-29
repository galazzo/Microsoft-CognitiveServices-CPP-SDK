#include <iostream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>

#include "microsoft/cognitive/cognitive.h"
#include "microsoft/cognitive/cv/cv.h"

using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision;

void print_usage() {
	printf("Usage: tag -i images/sample06.jpg | http://www.comstoso.com/sample.jpg -s=subscriptionKey\n");
}

int main(int argc, char **argv)
{
    cout << "Microsoft Cognitive Services in C++" << endl;
    cout << "Thumbnail" << endl;

	std::string subscriptionKeyFilePath = "subscriptionKey";
	std::string input;
	std::string projectid;

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

	if (isUrl(input))
	{
		Image image = GenerateThumbnail(input, 300, 200, true, ApiServerRegion::West_Europe, subscriptionKey);

		Microsoft::CognitiveServices::ComputerVision::ToFile("thumbnail.jpg", image);
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

		Image image = GenerateThumbnail(&wt, 300, 200, true, ApiServerRegion::West_Europe, subscriptionKey);

		Microsoft::CognitiveServices::ComputerVision::ToFile("thumbnail.jpg", image);
	}

    return 0;
}
