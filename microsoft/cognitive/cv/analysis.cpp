#include "microsoft/cognitive/cv/analysis.h"
#include <iostream>
#include <math.h> 
using namespace std;

Analysis::Analysis(void) {}
Analysis::~Analysis(){}

void Analysis::Serialize( Json::Value& root )
{
    // serialize primitives
    //root["testintA"] = m_nTestInt;
    //root["testfloatA"] = m_fTestFloat;
    //root["teststringA"] = m_TestString;
    //root["testboolA"] = m_bTestBool;
}

void Analysis::Deserialize( Json::Value& root )
{
	_requestId = root["requestId"].asString();
	
	if( root.isMember("metadata")) {
		const Json::Value jmetadata = root["metadata"];
		_metadata.width  = jmetadata["width"].asInt();
		_metadata.height = jmetadata["height"].asInt();
		_metadata.format = jmetadata["format"].asString();
	}
	
	if( root.isMember("description")) {
		const Json::Value jdtags = root["description"]["tags"];
		for ( int index = 0; index < jdtags.size(); ++index ) {
		   _description._tags.push_back(jdtags[index].asString());
		}

		const Json::Value jdcaptions = root["description"]["captions"];
		for ( int index = 0; index < jdcaptions.size(); ++index ) {
			Json::Value jdcaption = jdcaptions[index];
			Caption caption;
			caption.text = jdcaption["text"].asString();
			caption.confidence = jdcaption["confidence"].asDouble();

		   _description._captions.push_back(caption);
		}
	}    

    if( root.isMember("categories")) {
        const Json::Value jcategories = root["categories"];
        for ( int index = 0; index < jcategories.size(); ++index ) {
            Json::Value jcategory = jcategories[index];
            Category category;
            category.name = jcategory["name"].asString();
            category.score = jcategory["score"].asDouble();

            if(jcategory.isMember("detail")) {

                Json::Value jdetail = jcategory["detail"];

                if(jdetail.isMember("landmarks")) {
                    Json::Value jlandmarks = jdetail["landmarks"];
                    for ( int ilandmark = 0; ilandmark < jlandmarks.size(); ++ilandmark ) {
                        Json::Value jlandmark = jlandmarks[ilandmark];
                        Landmark landmark;
                        landmark.name = jlandmark["name"].asString();
                        landmark.confidence = jlandmark["confidence"].asDouble();
                        category.detail.landmarks.push_back(landmark);
                    }
                }

                if(jdetail.isMember("celebrities")) {
                    Json::Value jcelebrities = jdetail["celebrities"];
                    for ( int icelebrity = 0; icelebrity < jcelebrities.size(); ++icelebrity ) {
                        Json::Value jcelebrity = jcelebrities[icelebrity];

                        Celebrity celebrity;
                        celebrity.name = jcelebrity["name"].asString();
                        celebrity.confidence = jcelebrity["confidence"].asDouble();

                        Json::Value jfaceRectangle = jcelebrity["faceRectangle"];
                        celebrity.faceRectangle.left = jfaceRectangle["left"].asInt();
                        celebrity.faceRectangle.top = jfaceRectangle["top"].asInt();
                        celebrity.faceRectangle.width = jfaceRectangle["width"].asInt();
                        celebrity.faceRectangle.height = jfaceRectangle["height"].asInt();

                        category.detail.celebrities.push_back(celebrity);
                    }
                }

            }

            _categories.push_back(category);
        }
    }

	if( root.isMember("tags")) {
		const Json::Value jtags = root["tags"];
		for ( int index = 0; index < jtags.size(); ++index ) {
			Json::Value jtag = jtags[index];
			Tag tag;
			tag.name = jtag["name"].asString();
			tag.confidence = jtag["confidence"].asDouble();
		   _tags.push_back(tag);
		}
	}
	
	if( root.isMember("faces")) {
		const Json::Value jfaces = root["faces"];
		for ( int index = 0; index < jfaces.size(); ++index ) {
			Json::Value jface = jfaces[index];
			Face face;
			face.age = jface["age"].asInt();
			face.gender = jface["gender"].asString();
			
			Json::Value jfaceRectangle = jface["faceRectangle"];
			face.position.left = jfaceRectangle["left"].asInt();
			face.position.top = jfaceRectangle["top"].asInt();
			face.position.width = jfaceRectangle["width"].asInt();
			face.position.height = jfaceRectangle["height"].asInt();
			
			if( root.isMember("metadata")) {
				face.position.coveringArea = (int) ceil( (((double)(face.position.width*face.position.height)) /  ((double)(_metadata.width * _metadata.height))) * 100);
			} else {
				face.position.coveringArea = -1;
			}
			
		   _faces.push_back(face);
		}
	}	

	if( root.isMember("adult")) {
		const Json::Value jadult = root["adult"];		
		_adult.isAdultContent  = jadult["isAdultContent"].asBool();
		_adult.isRacyContent  = jadult["isRacyContent"].asBool();
		_adult.adultScore  = jadult["adultScore"].asDouble();
		_adult.racyScore  = jadult["racyScore"].asDouble();		
	}
	
	if( root.isMember("color")) {
		const Json::Value jcolor = root["color"];
		_color.dominantColorForeground  = jcolor["dominantColorForeground"].asString();
		_color.dominantColorBackground  = jcolor["dominantColorBackground"].asString();
		_color.accentColor = jcolor["accentColor"].asString();
		
		_color.isBWImg = jcolor["isBWImg"].asBool();
		
		const Json::Value jdominantColors = jcolor["dominantColors"];
		for ( int index = 0; index < jdominantColors.size(); ++index ) {
			_color.dominantColors.push_back(jdominantColors[index].asString());
		}		
	}
	
	if( root.isMember("imageType")) {
		const Json::Value jimageType = root["imageType"];
		_imageType.clipArtType = jimageType["clipArtType"].asBool();
		_imageType.lineDrawingType = jimageType["lineDrawingType"].asBool();
	}
		
}

Description Analysis::description() {
    return _description;
}

std::vector<Category> Analysis::categories() {
    return _categories;
}

const std::string Analysis::requestId(){
    return _requestId;
}

const Metadata Analysis::metadata(){
    return this->_metadata;
}

const Adult Analysis::adult(){
    return this->_adult;
}

const Color Analysis::color(){
    return this->_color;
}

void Analysis::debug() {

    std::cout << "Metadata:" << endl;
    std::cout << "\twidth: "<< _metadata.width << endl;
    std::cout << "\theight: "<< _metadata.height << endl;
    std::cout << "\tformat: "<< _metadata.format << endl;

    std::cout << "description " << endl;
    std::cout << "\ttags " << endl;
    for (std::vector<std::string>::iterator it = _description._tags.begin() ; it != _description._tags.end(); ++it) {
        std::cout << "\t\t" << *it << endl;
    }

    std::cout << "\tcaptions " << endl;
    for (std::vector<Caption>::iterator it = _description._captions.begin() ; it != _description._captions.end(); ++it) {
        std::cout << "\t\tcaption " << endl;
        std::cout << "\t\t\ttext: " << ((Caption)(*it)).text << endl;
        std::cout << "\t\t\tconfidence: " << ((Caption)(*it)).confidence << endl;
        std::cout << endl;
    }

	std::cout << "tags " << endl;
    for (std::vector<Tag>::iterator it = _tags.begin() ; it != _tags.end(); ++it) {
        std::cout << "\ttag " << endl;
        std::cout << "\t\tname: " << ((Tag)(*it)).name << endl;
        std::cout << "\t\tconfidence: " << ((Tag)(*it)).confidence << endl;
        std::cout << endl;
    }
	
	std::cout << "Adult" << endl;
    std::cout << "\tisAdultContent: "<< _adult.isAdultContent << endl;
    std::cout << "\tisRacyContent: "<< _adult.isRacyContent << endl;
    std::cout << "\tadultScore: "<< _adult.adultScore << endl;
	std::cout << "\tracyScore: "<< _adult.racyScore << endl;
	
	std::cout << "color " << endl;
	std::cout << "\tdominantColorForeground: " << _color.dominantColorForeground << endl;
	std::cout << "\tdominantColorBackground: " << _color.dominantColorBackground << endl;
	std::cout << "\taccentColor: " << _color.accentColor << endl;
	std::cout << "\tisBWImg " << _color.isBWImg << endl;	
	std::cout << "\tdominantColors" << endl;
    for (std::vector<std::string>::iterator it = _color.dominantColors.begin() ; it != _color.dominantColors.end(); ++it) {        
        std::cout << "\t\t " << (std::string)(*it) << endl;        
        std::cout << endl;
    }
	
	std::cout << "imageType " << endl;
	std::cout << "\tclipArtType: " << _imageType.clipArtType << endl;
	std::cout << "\tlineDrawingType: " << _imageType.lineDrawingType << endl;
	
	
	std::cout << "faces " << endl;
    for (std::vector<Face>::iterator it = _faces.begin() ; it != _faces.end(); ++it) {
        std::cout << "\tface " << endl;
        std::cout << "\t\tage: " << ((Face)(*it)).age << endl;
        std::cout << "\t\tgender: " << ((Face)(*it)).gender << endl;
		std::cout << "\t\t\tposition " << endl;
		std::cout << "\t\t\t\tleft " << ((Face)(*it)).position.left << endl;
		std::cout << "\t\t\t\ttop " << ((Face)(*it)).position.top << endl;
		std::cout << "\t\t\t\twidth " << ((Face)(*it)).position.width << endl;
		std::cout << "\t\t\t\theight " << ((Face)(*it)).position.height << endl;		
		std::cout << "\t\t\t\tcoveringArea " << ((Face)(*it)).position.coveringArea << endl;	
        std::cout << endl;
    }
	
    std::cout << "categories " << endl;
    for (std::vector<Category>::iterator it = _categories.begin() ; it != _categories.end(); ++it) {
        std::cout << "\tcategory " << endl;
        std::cout << "\t\tname: " << ((Category)(*it)).name << endl;
        std::cout << "\t\tscore: " << ((Category)(*it)).score << endl;

        Microsoft::CognitiveServices::ComputerVision::Detail detail = ((Category)(*it)).detail;

        std::cout << "\t\tdetail "  << endl;
        for (std::vector<Celebrity>::iterator itc = detail.celebrities.begin() ; itc != detail.celebrities.end(); ++itc) {
            std::cout << "\t\t\tCelebrities " << endl;
            std::cout << "\t\t\t\tname: " << ((Celebrity)(*itc)).name << endl;
            std::cout << "\t\t\t\tconfidence: " << ((Celebrity)(*itc)).confidence << endl;

            FaceRectangle frect = ((Celebrity)(*itc)).faceRectangle;
            std::cout << "\t\t\t\tface rectangle " << endl;
            std::cout << "\t\t\t\t\t\tleft: " << frect.left << endl;
            std::cout << "\t\t\t\t\t\ttop: " << frect.top << endl;
            std::cout << "\t\t\t\t\t\twidth: " << frect.width << endl;
            std::cout << "\t\t\t\t\t\theight: " << frect.height << endl;
        }

        for (std::vector<Landmark>::iterator itl = detail.landmarks.begin() ; itl != detail.landmarks.end(); ++itl) {
            std::cout << "\t\t\tLandmarks " << endl;
            std::cout << "\t\t\t\tname: " << ((Landmark)(*itl)).name << endl;
            std::cout << "\t\t\t\tconfidence: " << ((Landmark)(*itl)).confidence << endl;
        }
        std::cout << endl;
    }
}

Microsoft::CognitiveServices::ComputerVision::Analysis Microsoft::CognitiveServices::ComputerVision::describe(HttpContent* data, std::string subscriptionKey, std::string ContentType)
{
	const char* endpoint ="https://westeurope.api.cognitive.microsoft.com/vision/v1.0/describe";

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));
	//headers.insert(std::pair<std::string, std::string>("Content-Type", "application/octet-stream"));
	//headers.insert(std::pair<std::string, std::string>("Content-Type", "multipart/form-data"));

	string json = post(endpoint,"", &headers, data);
	Microsoft::CognitiveServices::ComputerVision::Analysis analysis;
	CJsonSerializer::Deserialize( &analysis, json );

	//std::cout << "Raw Json Input\n" << json << "\n\n";

	return analysis;
};

Microsoft::CognitiveServices::ComputerVision::Analysis Microsoft::CognitiveServices::ComputerVision::analyze(HttpContent* data, std::string subscriptionKey, std::string ContentType)
{
	//const char* endpoint ="https://westeurope.api.cognitive.microsoft.com/vision/v1.0/analyze";
	const char* endpoint ="https://westeurope.api.cognitive.microsoft.com/vision/v1.0/analyze?visualFeatures=Categories,Tags,Description,Faces,ImageType,Color,Adult&details=Landmarks,Celebrities&language=en";

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));
	//headers.insert(std::pair<std::string, std::string>("Content-Type", "application/octet-stream"));
	//headers.insert(std::pair<std::string, std::string>("Content-Type", "multipart/form-data"));

	string json = post(endpoint,"", &headers, data);
	Microsoft::CognitiveServices::ComputerVision::Analysis analysis;
	CJsonSerializer::Deserialize( &analysis, json );

	//std::cout << "Raw Json Input\n" << json << "\n\n";

	return analysis;
};
