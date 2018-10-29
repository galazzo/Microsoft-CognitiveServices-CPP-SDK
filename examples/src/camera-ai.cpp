   
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <json/json.h>

#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>

#include <iostream>
#include <fstream>


#include "microsoft/cognitive/cognitive.h"
#include "microsoft/cognitive/cv/analysis.h"
#include "microsoft/cognitive/cv/custom_vision_prediction.h"
#include "microsoft/cognitive/cv/ocr.h"

#include "gphoto2-cpp/gphoto2.h"
#include <unistd.h>
#include <getopt.h>

using namespace std;
using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision;
using namespace Gphoto2;

static char buffer[1024];

void print_usage() 
{
	printf("Usage: camera-ai -source url|image -i images/sample06.jpg|http://www.comstoso.com/sample.jpg -projectid <YOUR PROJECT ID>\n");
}

// https://askubuntu.com/questions/993876/gphoto2-could-not-claim-the-usb-device
// ps aux | grep gphoto
// peter    25802  2.1  0.1 302504  8736 ? Ssl  13 : 10   0 : 00 / usr / lib / gvfs / gvfs - gphoto2 - volume - monitor
// peter    25814  2.2  0.1 441508 11176 ? Sl   13 : 10   0 : 00 / usr / lib / gvfs / gvfsd - gphoto2 --spawner : 1.3 / org / gtk / gvfs / exec_spaw / 21
// peter    25835  0.0  0.0  22676  1096 pts / 0    S + 13 : 10   0 : 00 grep --color = auto gphoto

// Although not mentioned in glibc documentation or getopt man page, optional arguments to long style command line parameters require 'equals sign' (=). 
// Space separating the optional argument from the parameter does not work.


//  sudo apt-get install libcurl4-openssl-dev

int main(int argc, char **argv) 
{
	std::cout << "Camera AI" << endl;

	std::string subscriptionKeyFilePath = "subscriptionKey";
		
	std::string input;
	std::string model = "landmarks";
	std::string projectid;
	std::string api = "analyze";

	//Specifying the expected options
	static struct option long_options[] = {
		{"subscriptionKey",      optional_argument,       0,  's' },		
		{"model",    optional_argument, 0,  'm' },
		{"projectid",   optional_argument, 0,  'p' },
		{"api",   required_argument, 0,  'a' },
		{nullptr,           0,                 0,  0   }
	};

	int opt = 0;
	int long_index = 0;
	while ((opt = getopt_long_only(argc, argv, "", long_options, &long_index)) != -1)
	{
		switch (opt)
		{
		case 'a': api = optarg;
			break;
		case 's': subscriptionKeyFilePath = optarg;
			break;		
		case 'm': model = optarg;
			break;
		case 'p': projectid = optarg;
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
		
    std::vector<Gphoto2::DigitalCamera*> Cameras = Gphoto2::DigitalCamera::Autodetect();
	
    int		ret;

    CameraText	text;

    if(Cameras.size() > 0)
    {
        Gphoto2::DigitalCamera* Camera = Cameras.front();
		
        ret = Camera->Open();
        if ( ret < GP_OK) {
            std::cout << "cannot open camera " << Camera->Model() << std::endl;
            return -1;
        } else {
            std::cout << "camera " << Camera->Model() << " opened" << endl;
        }

		std::cout << "camera step #2" << endl;
        ret = Camera->CanonEnableCapture(FALSE);
        if ( ret < GP_OK) {
            std::cout << "cannot enable camera " << Camera->Model() << std::endl;
            return -1;
        } else {
            std::cout << "camera " << Camera->Model() << " enabled" << endl;
        }
		
        char* value = new char[256];
        Camera->GetConfigValueString((const char *)"viewfinder", &value);
        cout << "viewfinder: " << value << endl;

        Camera->GetConfigValueString((const char *)"capture", &value);
        cout << "capture: " << value << endl;

        Camera->GetConfigValueString((const char *)"ownername", &value);
        cout << "ownername: " << value << endl;

        Camera->GetConfigValueString((const char *)"iso", &value);
        cout << "iso: " << value << endl;

        Camera->GetConfigValueString((const char *)"aperture", &value);
        cout << "aperture: " << value << endl;

        Camera->GetConfigValueString((const char *)"uilock", &value);
        cout << "uilock: " << value << endl;

        Camera->GetConfigValueString((const char *)"eosremoterelease", &value);
        cout << "eosremoterelease: " << value << endl;

        // gphoto2 --set-config viewfinder=0
        struct HttpContent wt;
        Camera->CapturePreviewToMemory(&wt.buffer, &wt.size);

        //wt.readptr = body;
        //wt.sizeleft = strlen(body);
        cout << "image size: " << wt.size << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << wt.buffer << endl;
        cout << "--------------------------------------------------------" << endl;

		if (api == "analyze") 
		{
			Analysis id = Analyze(&wt, ApiServerRegion::West_Europe, subscriptionKey, "application/octet-stream");
			id.debug();
		}

		if (api == "describe")
		{
			Analysis id = Describe(&wt, ApiServerRegion::West_Europe, subscriptionKey, "application/octet-stream");
			id.debug();
		}
		
		if (api == "ocr")
		{
			Text::OcrResult id = Text::OCR(&wt, Microsoft::CognitiveServices::ApiServerRegion::West_Europe, subscriptionKey);
			id.debug();
		}

		if (api == "tags")
		{
			Analysis id = Tags(&wt, ApiServerRegion::West_Europe, subscriptionKey, "application/octet-stream");
			id.debug();
		}
		
		if (api == "custom-vision")
		{	
			Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction id = Microsoft::CognitiveServices::ComputerVision::CustomVision::PredictImageWithNoStore(&wt, projectid, ApiServerRegion::South_Central_US, subscriptionKey);
			id.debug();			
		}

		if (api == "face")
		{
			Json::Value options;

			options["returnFaceId"] = "true";
			options["returnFaceLandmarks"] = "true";
			
			cout << "analyzing image..." << endl;
			
			Face::Detect(&wt, options, ApiServerRegion::West_Europe, subscriptionKey);
			//id.debug();			
		}

		// After the shot ajust the settings according to the picture analysis
        //Camera->GetConfigValueString((const char *)"viewfinder", &value); cout << "viewfinder: " << value << endl;
        //Camera->GetConfigValueString((const char *)"iso", &value);        cout << "iso: " << value << endl;
		 
		// Then takes the full frame shot
        //Camera->CaptureToFile("foo.jpg");
    }

    sleep(1);

    return 0;
}
