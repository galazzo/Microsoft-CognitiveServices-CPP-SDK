#include <iostream>
#include "microsoft/cognitive/cv/cv.h"

using namespace std;

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
std::string Microsoft::CognitiveServices::ComputerVision::Face::Identify(HttpContent* data,
	ApiServerRegion region,
	std::string subscriptionKey,
	std::string ContentType)
{
	std::string endpoint = "https://" + ApiServer(region) + "/face/v1.0/identify";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint, "", &headers, data);
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
std::string Microsoft::CognitiveServices::ComputerVision::Face::Verify(HttpContent* data,
	ApiServerRegion region,
	std::string subscriptionKey,
	std::string ContentType)
{
	std::string endpoint = "https://" + ApiServer(region) + "/face/v1.0/verify";
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint, "", &headers, data);
	//std::cout << "Raw Json Input\n" << response.content << "\n\n";

	return response.content;
};

