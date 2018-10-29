#include <iostream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>

#include "microsoft/cognitive/cognitive.h"
#include "microsoft/cognitive/cv/recognize_text.h"

using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision;

void print_usage() {
	printf("Usage: tag -i images/sample06.jpg | http://www.comstoso.com/sample.jpg -s=subscriptionKey\n");
}

int main(int argc, char **argv)
{
    cout << "Microsoft Cognitive Services in C++" << endl;
	cout << "Recognize Text" << endl;

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

	HttpResponse operation_location;

	if (isUrl(input))
	{
		operation_location = Text::RecognizeText(input, ApiServerRegion::West_Europe, subscriptionKey);
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

		operation_location = Text::RecognizeText(&wt, ApiServerRegion::West_Europe, subscriptionKey);		
	}

	if (operation_location.status == 0) // CURLE_OK
	{
		bool wait = false;
		Text::RecognitionResult result;
		int retry = 1;

		do {

			cout << retry << " attempt: " << operation_location.content << endl;

			result = Text::RecognizeTextOperationResult(operation_location.content, subscriptionKey, "application/octet-stream");
			result.debug();

			if (result.status() == "Succeeded" || result.status() == "Failed") {
				wait = false;
			}
			else
			{
				wait = true;
				sleep(2);
			}

			retry++;

		} while (wait);
	}
	else {
		cout << endl << "status: " << operation_location.status << endl;
	}

    return 0;
}
