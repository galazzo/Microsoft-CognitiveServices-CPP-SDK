#include "microsoft/cognitive/cv/person.h"
#include <iostream>
#include <math.h> 
using namespace std;


void Microsoft::CognitiveServices::ComputerVision::Person::CreateGroup(HttpContent* data, 
																		std::string personGroupId, 
																		std::string subscriptionKey,
																		std::string ContentType)
{	
	std::string endpoint = "https://westeurope.api.cognitive.microsoft.com/face/v1.0/persongroups/"+personGroupId;
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";

};

void Microsoft::CognitiveServices::ComputerVision::Person::TrainGroup(std::string personGroupId,
																	std::string subscriptionKey,
																	std::string ContentType)
{
	std::string endpoint = "https://westeurope.api.cognitive.microsoft.com/face/v1.0/persongroups/"+personGroupId+"/train";

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
void Microsoft::CognitiveServices::ComputerVision::Person::CreatePerson(HttpContent* data, 
																		std::string personGroupId,
																		std::string subscriptionKey, 
																		std::string ContentType)
{	
	std::string endpoint="https://westeurope.api.cognitive.microsoft.com/face/v1.0/persongroups/"+personGroupId+"/persons";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";
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

	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";
};

/*
{    
    "personGroupId":"sample_group",
    "faceIds":[
        "c5c24a82-6845-4031-9d5d-978df9175426",
        "65d083d4-9447-47d1-af30-b626144bf0fb"
    ],
    "maxNumOfCandidatesReturned":1,
    "confidenceThreshold": 0.5
}
*/
std::string Microsoft::CognitiveServices::ComputerVision::Person::Identify(HttpContent* data, std::string subscriptionKey, std::string ContentType)
{
	std::string endpoint = 	"https://westeurope.api.cognitive.microsoft.com/face/v1.0/identify";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";
	return response.content;
};


/*
{
    "faceId":"c5c24a82-6845-4031-9d5d-978df9175426",
    "personId":"815df99c-598f-4926-930a-a734b3fd651c",
    "personGroupId":"sample_group"
}
*/
std::string Microsoft::CognitiveServices::ComputerVision::Person::Verify(HttpContent* data, std::string subscriptionKey, std::string ContentType)
{
	std::string endpoint = 	"https://westeurope.api.cognitive.microsoft.com/face/v1.0/verify";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint,"", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";
	
	return response.content;
};