#include "domainspecificcontent.h"
#include <iostream>
using namespace std;

Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent::DomainSpecificContent()
{

}

Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent::~DomainSpecificContent()
{

}

const std::string Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent::requestId(){
    return _requestId;
}


std::vector<Microsoft::CognitiveServices::ComputerVision::Celebrity> Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent::celebrities() {
    return _celebrities;
}


std::vector<Microsoft::CognitiveServices::ComputerVision::Landmark> Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent::landmarks() {
    return _landmarks;
}

void Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent::Serialize( Json::Value& root )
{
}

void Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent::Deserialize( Json::Value& root )
{
	response.status_code = root.isMember("statusCode") ? root["statusCode"].asInt() : 0;
	response.message = root.isMember("message") ? root["message"].asString() : std::string();

    _requestId = root["requestId"].asString();

    if( root.isMember("metadata")) {
        const Json::Value jmetadata = root["metadata"];
        _metadata.width  = jmetadata["width"].asInt();
        _metadata.height = jmetadata["height"].asInt();
        _metadata.format = jmetadata["format"].asString();
    }

    if( root.isMember("result"))
    {
        const Json::Value jresult = root["result"];

        if( jresult.isMember("landmarks"))
        {
            const Json::Value jlandmarks = jresult["landmarks"];
            cout << jlandmarks.size() << endl;

            for ( int index = 0; index < jlandmarks.size(); ++index )
            {
                Json::Value jlandmark = jlandmarks[index];
                Landmark landmark;

                landmark.name = jlandmark["name"].asString();
                landmark.confidence = jlandmark["confidence"].asDouble();

               _landmarks.push_back(landmark);

               cout << landmark.name << endl;
            }
        }

        if( jresult.isMember("celebrities"))
        {
            const Json::Value jcelebrities = jresult["celebrities"];
            for ( int index = 0; index < jcelebrities.size(); ++index ) {
                Json::Value jcelebrity = jcelebrities[index];

                Celebrity celebrity;

                if(jcelebrity.isMember("name"))
                {
                    celebrity.name = jcelebrity["name"].asString();
                }

				if (jcelebrity.isMember("confidence"))
				{
					celebrity.confidence = jcelebrity["confidence"].asDouble();
				}

                if(jcelebrity.isMember("faceRectangle"))
                {
                    Json::Value jfaceRectangle = jcelebrity["faceRectangle"];
                    celebrity.faceRectangle.left = jfaceRectangle["left"].asInt();
                    celebrity.faceRectangle.top = jfaceRectangle["top"].asInt();
                    celebrity.faceRectangle.width = jfaceRectangle["width"].asInt();
                    celebrity.faceRectangle.height = jfaceRectangle["height"].asInt();
                }

               _celebrities.push_back(celebrity);
            }
        }
    }
}

void Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent::debug()
{
	std::cout << "status code: " << response.status_code << endl;
	std::cout << "message: " << response.message << endl;
	std::cout << endl;

    std::cout << "requestId: " << _requestId << endl;

    std::cout << "Metadata:" << endl;
    std::cout << "\twidth: "<< _metadata.width << endl;
    std::cout << "\theight: "<< _metadata.height << endl;
    std::cout << "\tformat: "<< _metadata.format << endl;

    std::cout << "\tlandmarks " << endl;
    for (std::vector<Landmark>::iterator it = _landmarks.begin() ; it != _landmarks.end(); ++it) {
        std::cout << "\t\tlandmark " << endl;
        std::cout << "\t\t\tname: " << ((Landmark)(*it)).name << endl;
        std::cout << "\t\t\tconfidence: " << ((Landmark)(*it)).confidence << endl;
        std::cout << endl;
    }

    std::cout << "\tcelebrities " << endl;
    for (std::vector<Celebrity>::iterator it = _celebrities.begin() ; it != _celebrities.end(); ++it) {
        std::cout << "\t\tcelebrity " << endl;
        std::cout << "\t\t\tname: " << ((Celebrity)(*it)).name << endl;
		std::cout << "\t\t\tconfidence: " << ((Celebrity)(*it)).confidence << endl;
        std::cout << "\t\t\tfaceRectangle " << endl;
        std::cout << "\t\t\t\tleft " << ((Celebrity)(*it)).faceRectangle.left << endl;
        std::cout << "\t\t\t\ttop " << ((Celebrity)(*it)).faceRectangle.top << endl;
        std::cout << "\t\t\t\twidth " << ((Celebrity)(*it)).faceRectangle.width << endl;
        std::cout << "\t\t\t\theight " << ((Celebrity)(*it)).faceRectangle.height << endl;
        std::cout << endl;
    }
}

Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent Microsoft::CognitiveServices::ComputerVision::RecognizeDomainSpecificContent(HttpContent* data, std::string model, ApiServerRegion region, std::string subscriptionKey, std::string ContentType)
{
    std::string endpoint = "https://" + ApiServer(region) + "/vision/v2.0/models/" + model + "/analyze";

    std::map<string, string> headers;

    headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
    headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

    HttpResponse response = post(endpoint,"", &headers, data);
    Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent analysis;
    CJsonSerializer::Deserialize( &analysis, response.content );

    //std::cout << "Raw Json Input\n" << response.content << "\n\n";

    return analysis;
};

Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent Microsoft::CognitiveServices::ComputerVision::RecognizeDomainSpecificContent(std::string url, std::string model, ApiServerRegion region, std::string subscriptionKey, std::string ContentType)
{
	std::string endpoint = "https://" + ApiServer(region) + "/vision/v2.0/models/" + model + "/analyze";

	HttpContent data;
	std::string json = "{\"url\": \"" + url + "\"}";
	data.buffer = new char[json.size() + 1];
	strcpy(data.buffer, json.c_str());
	data.size = json.size() + 1;

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint, "", &headers, &data);
	Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent analysis;
	CJsonSerializer::Deserialize(&analysis, response.content);

	//std::cout << "Raw Json Input\n" << response.content << "\n\n";

	return analysis;
};

std::vector<Microsoft::CognitiveServices::ComputerVision::DomainModel> Microsoft::CognitiveServices::ComputerVision::DomainSpecificModels(ApiServerRegion region, std::string subscriptionKey, std::string ContentType)
{
    std::string endpoint = "https://" + ApiServer(region) + "/vision/v2.0/models/";

    std::map<string, string> headers;

    headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
    headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

    HttpResponse response = get(endpoint, &headers);
    //std::cout << "Raw Json Input\n" << response.content << "\n\n";

    std::vector<Microsoft::CognitiveServices::ComputerVision::DomainModel> result;

    Json::Value jroot;
    Json::Reader reader;

    if ( !reader.parse(response.content, jroot) ) {
        std::cout << "failed to parse json" << endl;;
       return result;
    }

    if( jroot.isMember("models"))
    {
        for ( int index = 0; index < jroot["models"].size(); ++index )
        {
            Json::Value jmodel = jroot["models"][index];

            Microsoft::CognitiveServices::ComputerVision::DomainModel dm;

            if( jmodel.isMember("name") ) {
                dm.name = jmodel["name"].asString();
            }

            if( jmodel.isMember("categories") ) {
                for ( int sindex = 0; sindex < jmodel["categories"].size(); ++sindex ) {
                    dm.categories.push_back(jmodel["categories"][sindex].asString());
                }
            }

            result.push_back(dm);
        }
    }

    return result;
};

void Microsoft::CognitiveServices::ComputerVision::DomainModel::debug() {

    std::cout << "model " << endl;
    std::cout << "\tname " << name << endl;
    std::cout << "\tcategories " << endl;

    for (std::vector<std::string>::iterator it = categories.begin() ; it != categories.end(); ++it) {
        std::cout << "\t\t " << ((std::string)(*it)) << endl;
    }

    std::cout << endl;
}
