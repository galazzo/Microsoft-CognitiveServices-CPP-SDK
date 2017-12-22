#include "microsoft/cognitive/cv/person.h"
#include <iostream>
#include <math.h> 
using namespace std;


void Microsoft::CognitiveServices::ComputerVision::Person::CreateGroup(HttpContent* data, std::string personGroupdId, std::string subscriptionKey, std::string ContentType)
{	
	std::string endpoint = "https://westeurope.api.cognitive.microsoft.com/face/v1.0/persongroups/"+personGroupId;
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	string json = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << json << "\n\n";

	return analysis;
};

void Microsoft::CognitiveServices::ComputerVision::Person::TrainGroup(std::string personGroupdId,
																	std::string subscriptionKey,
																	std::string ContentType)
{
	std::string endpoint = "https://westeurope.api.cognitive.microsoft.com/face/v1.0/persongroups/"+personGroupId+"/train";

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpContent* data = new HttpContent;
	data->buffer = new char[1]; 
	data->buffer[0] = 0;
	data->size = 0;
	
	string json = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << json << "\n\n";

	return analysis;
};

/*
{
    "name":"Person1",
    "userData":"User-provided data attached to the person"
}
*/
void Microsoft::CognitiveServices::ComputerVision::Person::CreatePerson(HttpContent* data, 
																		std::string personGroupdId,
																		std::string subscriptionKey, 
																		std::string ContentType)
{	
	std::string endpoint "https://westeurope.api.cognitive.microsoft.com/face/v1.0/persongroups/"+personGroupId+"/persons";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	string json = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << json << "\n\n";

	return analysis;
};


void Microsoft::CognitiveServices::ComputerVision::Person::AddPersonFace(HttpContent* data,
																		std::string personGroupId,
																		std::string personId,
																		std::string subscriptionKey, 
																		std::string ContentType)
{
	std::string endpoint = 	"https://westus.api.cognitive.microsoft.com/face/v1.0/persongroups/"+personGroupId+"/persons/"+personId+"/persistedFaces";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	string json = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << json << "\n\n";

	return analysis;
};