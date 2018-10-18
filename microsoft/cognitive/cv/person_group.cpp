#include <iostream>
#include <math.h> 
#include "microsoft/cognitive/cv/cv.h"

using namespace std;

void Microsoft::CognitiveServices::ComputerVision::PersonGroup::Create(HttpContent* data, 
																		std::string personGroupId, 
                                                                        ApiServerRegion region,
																		std::string subscriptionKey,
																		std::string ContentType)
{	
    std::string endpoint = "https://" + ApiServer(region) + "/face/v1.0/persongroups/"+personGroupId;
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";

};

void Microsoft::CognitiveServices::ComputerVision::PersonGroup::Train(std::string personGroupId,
                                                                      ApiServerRegion region,
                                                                      std::string subscriptionKey,
                                                                      std::string ContentType)
{
    std::string endpoint = "https://" + ApiServer(region) + "/face/v1.0/persongroups/"+personGroupId+"/train";

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpContent* data = new HttpContent;
	data->buffer = new char[1]; 	
	data->size = 0;
	
	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";
};

/*
{
    "name":"Person1",
    "userData":"User-provided data attached to the person"
}
*/
void Microsoft::CognitiveServices::ComputerVision::PersonGroup::Person::Create(HttpContent* data, 
																		std::string personGroupId,
                                                                        ApiServerRegion region,
																		std::string subscriptionKey, 
																		std::string ContentType)
{	
    std::string endpoint = "https://" + ApiServer(region) +"/face/v1.0/persongroups/"+personGroupId+"/persons";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";
};


void Microsoft::CognitiveServices::ComputerVision::PersonGroup::Person::AddFace(HttpContent* data,
																		std::string personGroupId,
																		std::string personId,
                                                                        ApiServerRegion region,
																		std::string subscriptionKey, 
																		std::string ContentType)
{
    std::string endpoint = "https://" + ApiServer(region) + "/face/v1.0/persongroups/"+personGroupId+"/persons/"+personId+"/persistedFaces";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";
};

