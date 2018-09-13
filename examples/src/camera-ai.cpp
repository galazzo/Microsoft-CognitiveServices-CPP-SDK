   
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
#include "gphoto2-cpp/gphoto2.h"

using namespace std;
using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision;
using namespace Gphoto2;

static char buffer[1024];

//  sudo apt-get install libcurl4-openssl-dev
int main(int argc, char **argv) 
{
    std::vector<Gphoto2::DigitalCamera> Cameras = Gphoto2::DigitalCamera::Autodetect();

    int		ret;

    //const char *camera_model = NULL;
    //const char* camera_port = NULL;

    CameraText	text;

    if(Cameras.size() > 0)
    {        
        Gphoto2::DigitalCamera Camera = Cameras.front();
        ret = Camera.Open();
        if ( ret < GP_OK) {
            std::cout << "cannot open camera " << Camera.Model() << std::endl;
            return -1;
        } else {
            std::cout << "camera " << Camera.Model() << " opened" << endl;
        }

        ret = Camera.CanonEnableCapture(FALSE);
        if ( ret < GP_OK) {
            std::cout << "cannot enable camera " << Camera.Model() << std::endl;
            return -1;
        } else {
            std::cout << "camera " << Camera.Model() << " enabled" << endl;
        }

        std::ifstream subscriptionKeyFile;
        std::string subscriptionKey;
        subscriptionKeyFile.open("subscriptionKey");
        std::getline(subscriptionKeyFile, subscriptionKey);
        subscriptionKeyFile.close();

        char* value = new char[256];
        Camera.GetConfigValueString((const char *)"viewfinder", &value);
        cout << "viewfinder: " << value << endl;

        Camera.GetConfigValueString((const char *)"capture", &value);
        cout << "capture: " << value << endl;

        Camera.GetConfigValueString((const char *)"ownername", &value);
        cout << "ownername: " << value << endl;

        Camera.GetConfigValueString((const char *)"iso", &value);
        cout << "iso: " << value << endl;

        Camera.GetConfigValueString((const char *)"aperture", &value);
        cout << "aperture: " << value << endl;

        Camera.GetConfigValueString((const char *)"uilock", &value);
        cout << "uilock: " << value << endl;

        Camera.GetConfigValueString((const char *)"eosremoterelease", &value);
        cout << "eosremoterelease: " << value << endl;

        // gphoto2 --set-config viewfinder=0
        struct HttpContent wt;
        Camera.CapturePreviewToMemory(&wt.buffer, &wt.size);

        //wt.readptr = body;
        //wt.sizeleft = strlen(body);
        cout << "image size: " << wt.size << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << wt.buffer << endl;
        cout << "--------------------------------------------------------" << endl;

        Analysis id = Analyze(&wt, ApiServerRegion::West_Europe, subscriptionKey, "application/octet-stream");
        id.debug();
		
        Camera.GetConfigValueString((const char *)"viewfinder", &value); cout << "viewfinder: " << value << endl;
        Camera.GetConfigValueString((const char *)"iso", &value);        cout << "iso: " << value << endl;
		
        //struct HttpContent wt2;
        //capture_to_memory(camera, context, &wt2.buffer, &wt2.size);
        Camera.CaptureToFile("foo.jpg");
    }

    sleep(1);

    return 0;
}
