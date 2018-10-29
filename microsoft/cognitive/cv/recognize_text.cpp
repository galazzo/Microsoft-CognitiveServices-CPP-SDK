#include "microsoft/cognitive/cv/recognize_text.h"
#include <iostream>
#include <math.h> 
using namespace std;

Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult::RecognitionResult(void) {}
Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult::~RecognitionResult(void) {}

const std::string Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult::status()
{
    return _status;
}

void Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult::Serialize( Json::Value& root )
{
   
}

void Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult::Deserialize( Json::Value& root )
{
	response.status_code = root.isMember("statusCode") ? root["statusCode"].asInt() : 0;
	response.message = root.isMember("message") ? root["message"].asString() : std::string();

    _status = root["status"].asString();
	
    if( root.isMember("recognitionResult")) {
        const Json::Value jrecognitionResult = root["recognitionResult"];

        if( jrecognitionResult.isMember("lines"))
        {
            Json::Value jlines = jrecognitionResult["lines"];
            for ( int iline = 0; iline < jlines.size(); ++iline )
            {
                Json::Value jline = jlines[iline];

                Microsoft::CognitiveServices::ComputerVision::Text::Line line;

                if( jline.isMember("text")) {
                    line.text = jline["text"].asString();
                }

                if( jline.isMember("boundingBox"))
                {
                   // cout << "boundingBox " << endl;
                    for ( int ibbox = 0; ibbox < 8; ++ibbox ) {
                        line.boundingBox[ibbox] = jline["boundingBox"][ibbox].asInt();
                        //cout << line.boundingBox[ibbox] << " ";
                    }
                    //cout << endl;
                }

                if( jline.isMember("words"))
                {
                    Json::Value jwords = jline["words"];
                    for ( int iword = 0; iword < jwords.size(); ++iword )
                    {
                        Json::Value jword = jwords[iword];

                        Microsoft::CognitiveServices::ComputerVision::Text::Word word;
                        if( jword.isMember("text"))
                        {
                            word.text = jword["text"].asString();
                        }

                        if( jword.isMember("boundingBox")) {
                            // cout << "boundingBox " << endl;
                             for ( int ibbox = 0; ibbox < 8; ++ibbox ) {
                                 word.boundingBox[ibbox] = jword["boundingBox"][ibbox].asInt();
                                 //cout << line.boundingBox[ibbox] << " ";
                             }
                             //cout << endl;
                        }

                        line.words.push_back(word);
                    }
                }

                _lines.push_back(line);
            }
        }
    }
}


std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Line> Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult::lines() {
    return _lines;
}

void Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult::debug()
{
	std::cout << "status code: " << response.status_code << endl;
	std::cout << "message: " << response.message << endl;
	std::cout << endl;

    std::cout << "status " << _status << endl;

    std::cout << "recognitionResult" << endl;
    std::cout << "\tlines" << endl;
    for (std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Line>::iterator it = _lines.begin() ; it != _lines.end(); ++it)
    {
        Microsoft::CognitiveServices::ComputerVision::Text::Line line = ((Microsoft::CognitiveServices::ComputerVision::Text::Line)(*it));
        std::cout << "\t\ttext " << line.text << endl;

        std::string sboundingBox;
        for(int i=0; i < 8; i++) {
            sboundingBox += std::to_string(line.boundingBox[i]);
            if( i < 7)
                sboundingBox += ", ";
        }
        std::cout << "\t\tboundingBox [" << sboundingBox << "]" << endl;

        std::cout << "\t\twords" << endl;
        for (std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Word>::iterator it = line.words.begin() ; it != line.words.end(); ++it)
        {
            Microsoft::CognitiveServices::ComputerVision::Text::Word word = ((Microsoft::CognitiveServices::ComputerVision::Text::Word)(*it));
            std::cout << "\t\t\ttext " << word.text << endl;

            std::string sboundingBox;
            for(int i=0; i < 8; i++) {
                sboundingBox += std::to_string(word.boundingBox[i]);
                if( i < 7)
                    sboundingBox += ", ";
            }
            std::cout << "\t\t\tboundingBox [" << sboundingBox << "]" << endl;
            std::cout << endl;
        }

        std::cout << endl;
    }
}

HttpResponse Microsoft::CognitiveServices::ComputerVision::Text::RecognizeText(HttpContent* data, ApiServerRegion region, std::string subscriptionKey, std::string ContentType)
{
    std::string endpoint = "https://" + ApiServer(region) + "/vision/v2.0/recognizeText?mode=Handwritten";

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint,"", &headers, data);	

    std::map<std::string, std::string>::iterator it;
		
    if ( (it=response.headers.find("Operation-Location")) != response.headers.end() )
    {
        // found
        response.content = it->second;
    } else
    {
        // not found
        std::cout << "Operation-Location header not found";
    }

    return response;
};

HttpResponse Microsoft::CognitiveServices::ComputerVision::Text::RecognizeText(std::string url, ApiServerRegion region, std::string subscriptionKey, std::string ContentType)
{
	std::string endpoint = "https://" + ApiServer(region) + "/vision/v2.0/recognizeText?mode=Handwritten";

	HttpContent data;
	std::string json = "{\"url\": \"" + url + "\"}";
	data.buffer = new char[json.size() + 1];
	strcpy(data.buffer, json.c_str());
	data.size = json.size() + 1;

	std::map<string, string> headers;

	headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
	headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

	HttpResponse response = post(endpoint, "", &headers, &data);

	std::map<std::string, std::string>::iterator it;

	if ((it = response.headers.find("Operation-Location")) != response.headers.end())
	{
		// found
		response.content = it->second;
	}
	else
	{
		// not found
		std::cout << "Operation-Location header not found";
	}

	return response;
};

Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult Microsoft::CognitiveServices::ComputerVision::Text::RecognizeTextOperationResult(std::string url, std::string subscriptionKey, std::string ContentType)
{   
    std::map<string, string> headers;

    headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
    headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

    Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult analysis;

    HttpResponse response = get(url, &headers);
    CJsonSerializer::Deserialize( &analysis, response.content );

    return analysis;
};
