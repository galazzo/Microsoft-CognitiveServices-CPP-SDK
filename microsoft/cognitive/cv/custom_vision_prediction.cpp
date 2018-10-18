#include "microsoft/cognitive/cv/custom_vision_prediction.h"
#include <iostream>
#include <algorithm> 
#include <cstring>

using namespace std;
using namespace Microsoft::CognitiveServices::ComputerVision::CustomVision;

Prediction::Prediction(void) {}
Prediction::~Prediction(){}

void Prediction::Serialize( Json::Value& root )
{
    
}

void Prediction::Deserialize( Json::Value& root )
{
	_id = root.isMember("id") ? root["id"].asString() : std::string();
	_project = root.isMember("project") ? root["project"].asString() : std::string();
	_iteration = root.isMember("iteration") ? root["iteration"].asString() : std::string();
	_created = root.isMember("created") ? root["created"].asString() : std::string();
	
    if(root.isMember("predictions")) {
        const Json::Value jpredictions = root["predictions"];
        for ( int index = 0; index < jpredictions.size(); ++index ) {
            Json::Value jprediction = jpredictions[index];
            PredictionBox prediction;
			prediction.propability = jprediction["probability"].asDouble();
			prediction.tagId = jprediction["tagId"].asString();
			prediction.tagName = jprediction["tagName"].asString();			

            if(jprediction.isMember("boundingBox"))
			{
                Json::Value jboundingBox = jprediction["boundingBox"];
				prediction.region.left = jboundingBox["left"].asDouble();
				prediction.region.top = jboundingBox["top"].asDouble();
				prediction.region.width = jboundingBox["width"].asDouble();
				prediction.region.height = jboundingBox["height"].asDouble();
            }

            _predictionBoxes.push_back(prediction);
        }
    }
}

const std::string Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction::id()
{
	return this->_id;
}

const std::string Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction::project()
{
	return this->_project;
}

const std::string Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction::iteration()
{
	return this->_iteration;
}

const std::string Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction::created()
{
	return this->_created;
}

const std::vector<PredictionBox>& Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction::predictions() const
{
	return this->_predictionBoxes;
}

void Prediction::debug()
{	
	std::cout << "id: " << _id << endl;
	std::cout << "project: " << _project << endl;
	std::cout << "iteration: " << _iteration << endl;
	std::cout << "created: " << _created << endl;

	std::cout << "Predictions: " << endl;	
	for (std::vector<PredictionBox>::iterator it = _predictionBoxes.begin(); it != _predictionBoxes.end(); ++it) 
	{
		std::cout << endl;
		std::cout << "\tpropability: " << it->propability << endl;
		std::cout << "\ttagId: " << it->tagId << endl;
		std::cout << "\ttagName:" << it->tagName << endl;
		std::cout << "\tBoundingBox:" << endl;
		std::cout << "\t\tLeft: " << it->region.left << endl;
		std::cout << "\t\tTop: " << it->region.top << endl;
		std::cout << "\t\tWidth: " << it->region.width << endl;
		std::cout << "\t\tHeight: " << it->region.height << endl;
	}
}

Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction Microsoft::CognitiveServices::ComputerVision::CustomVision::PredictImage(HttpContent * data, std::string project_id, ApiServerRegion region, std::string predictionKey, std::string ContentType)
{
	std::string endpoint = "https://" + ApiServer(region) + "/customvision/v2.0/Prediction/" + project_id + "/image";  // [?iterationId][&application]
		
	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Prediction-key", predictionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint, "", &headers, data);
	Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction Prediction;
	CJsonSerializer::Deserialize( &Prediction, response.content );

	//std::cout << "Raw Json Input\n" << response.content << "\n\n";

	return Prediction;
}

Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction Microsoft::CognitiveServices::ComputerVision::CustomVision::PredictImageWithNoStore(HttpContent * data, std::string project_id, ApiServerRegion region, std::string predictionKey, std::string ContentType)
{
	std::string endpoint = "https://" + ApiServer(region) + "/customvision/v2.0/Prediction/" + project_id + "/image/nostore"; // [?iterationId][&application]

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Prediction-key", predictionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint, "", &headers, data);
	Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction Prediction;
	CJsonSerializer::Deserialize(&Prediction, response.content);

	//std::cout << "Raw Json Input\n" << response.content << "\n\n";

	return Prediction;
}

Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction Microsoft::CognitiveServices::ComputerVision::CustomVision::PredictImageUrl(std::string url, std::string project_id, ApiServerRegion region, std::string predictionKey, std::string ContentType)
{	
    std::string endpoint = "https://" + ApiServer(region) + "/customvision/v2.0/Prediction/"+ project_id +"/url";  // [?iterationId][&application]

	HttpContent data;
	std::string json = "{\"Url\": \""+ url +"\"}";
	data.buffer = new char[json.size() + 1];
	strcpy(data.buffer, json.c_str());	
	data.size = json.size() + 1;

    std::map<string, string> headers;

    headers.insert(std::pair<std::string, std::string>("Prediction-key", predictionKey));
    headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

    HttpResponse response = post(endpoint,"", &headers, &data);
    Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction Prediction;
    CJsonSerializer::Deserialize( &Prediction, response.content );

    //std::cout << "Raw Json Input\n" << response.content << "\n\n";

    return Prediction;
}

Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction Microsoft::CognitiveServices::ComputerVision::CustomVision::PredictImageUrlWithNoStore(std::string url, std::string project_id, ApiServerRegion region, std::string predictionKey, std::string ContentType)
{	
	std::string endpoint = "https://" + ApiServer(region) + "/customvision/v2.0/Prediction/" + project_id + "/url/nostore"; // [? iterationId][&application]

	HttpContent data;
	std::string json = "{\"Url\": \"" + url + "\"}";
	data.buffer = new char[json.size() + 1];
	strcpy(data.buffer, json.c_str());
	data.size = json.size() + 1;

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Prediction-key", predictionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint, "", &headers, &data);
	Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction Prediction;
	CJsonSerializer::Deserialize(&Prediction, response.content);

	//std::cout << "Raw Json Input\n" << response.content << "\n\n";

	return Prediction;
}