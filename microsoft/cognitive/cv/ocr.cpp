#include "microsoft/cognitive/cv/ocr.h"
#include "microsoft/utils/string.h"
#include <iostream>
#include <math.h>
using namespace std;
using namespace Microsoft::CognitiveServices::Utils::String;

Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::OcrResult(void) {}
Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::~OcrResult(void) {}

const std::string Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::language()
{
    return _language;
}

double Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::textAngle()
{
    return _textAngle;
}

const std::string Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::orientation()
{
    return _orientation;
}

std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Region> Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::regions() {
    return _regions;
}

void Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::Serialize( Json::Value& root )
{

}

void Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::Deserialize( Json::Value& root )
{
    _language = root["language"].asString();
    _orientation = root["orientation"].asString();
    _textAngle = root["textAngle"].asDouble();

    if( root.isMember("regions")) {
        const Json::Value jregions = root["regions"];

        for ( int iregion = 0; iregion < jregions.size(); ++iregion ) {

            Microsoft::CognitiveServices::ComputerVision::Text::Region region;
            const Json::Value jregion = jregions[iregion];

            if( jregion.isMember("boundingBox"))
            {
                int ibbox = 0;
                std::vector<std::string> vbbox = Split(jregion["boundingBox"].asString(), ',');
                for (std::vector<std::string>::iterator itbox = vbbox.begin() ; itbox != vbbox.end(); ++itbox) {
                    region.boundingBox[ibbox] = std::stoi(*itbox);
                    ++ibbox;
                }

                for ( ; ibbox < 8; ++ibbox ) {
                    region.boundingBox[ibbox] = -1;
                }
            }

            if( jregion.isMember("lines"))
            {
                Json::Value jlines = jregion["lines"];
                for ( int iline = 0; iline < jlines.size(); ++iline )
                {
                    Json::Value jline = jlines[iline];

                    Microsoft::CognitiveServices::ComputerVision::Text::Line line;

                    if( jline.isMember("text")) {
                        line.text = jline["text"].asString();
                    }

                    if( jline.isMember("boundingBox"))
                    {
                        int ibbox = 0;
                        std::vector<std::string> vbbox = Split(jline["boundingBox"].asString(), ',');
                        for (std::vector<std::string>::iterator itbox = vbbox.begin() ; itbox != vbbox.end(); ++itbox) {
                            line.boundingBox[ibbox] = std::stoi(*itbox);
                            ++ibbox;
                        }

                        for ( ; ibbox < 8; ++ibbox ) {
                            line.boundingBox[ibbox] = -1;
                        }
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

                            if( jword.isMember("boundingBox"))
                            {
                                int ibbox = 0;
                                std::vector<std::string> vbbox = Split(jword["boundingBox"].asString(), ',');
                                for (std::vector<std::string>::iterator itbox = vbbox.begin() ; itbox != vbbox.end(); ++itbox) {
                                    word.boundingBox[ibbox] = std::stoi(*itbox);
                                    ++ibbox;
                                }

                                for ( ; ibbox < 8; ++ibbox ) {
                                    word.boundingBox[ibbox] = -1;
                                }
                            }

                            line.words.push_back(word);
                        }
                    }

                    region.lines.push_back(line);
                }
            }

            _regions.push_back(region);
        }
    }

}

void Microsoft::CognitiveServices::ComputerVision::Text::OcrResult::debug()
{
    std::cout << "language " << _language << endl;
    std::cout << "orientation " << _orientation << endl;
    std::cout << "textAngle " << std::to_string(_textAngle) << endl;

    std::cout << "\tregions" << endl;
    for (std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Region>::iterator it = _regions.begin() ; it != _regions.end(); ++it)
    {
        Microsoft::CognitiveServices::ComputerVision::Text::Region region = ((Microsoft::CognitiveServices::ComputerVision::Text::Region)(*it));

        std::string sboundingBox;
        for(int i=0; i < 8; i++) {
            sboundingBox += std::to_string(region.boundingBox[i]);
            if( i < 7)
                sboundingBox += ", ";
        }
        std::cout << "\t\tboundingBox [" << sboundingBox << "]" << endl;

        for (std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Line>::iterator itline = region.lines.begin() ; itline != region.lines.end(); ++itline)
        {
            Microsoft::CognitiveServices::ComputerVision::Text::Line line = ((Microsoft::CognitiveServices::ComputerVision::Text::Line)(*itline));
            //std::cout << "\t\t\ttext " << line.text << endl;

            std::string sboundingBox;
            for(int i=0; i < 8; i++) {
                sboundingBox += std::to_string(line.boundingBox[i]);
                if( i < 7)
                    sboundingBox += ", ";
            }
            std::cout << "\t\t\tboundingBox [" << sboundingBox << "]" << endl;

            std::cout << "\t\t\twords" << endl;
            for (std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Word>::iterator it = line.words.begin() ; it != line.words.end(); ++it)
            {
                Microsoft::CognitiveServices::ComputerVision::Text::Word word = ((Microsoft::CognitiveServices::ComputerVision::Text::Word)(*it));
                std::cout << "\t\t\t\ttext " << word.text << endl;

                std::string sboundingBox;
                for(int i=0; i < 8; i++) {
                    sboundingBox += std::to_string(word.boundingBox[i]);
                    if( i < 7)
                        sboundingBox += ", ";
                }
                std::cout << "\t\t\t\tboundingBox [" << sboundingBox << "]" << endl;
                std::cout << endl;
            }

        }
        std::cout << endl;
    }
}

Microsoft::CognitiveServices::ComputerVision::Text::OcrResult Microsoft::CognitiveServices::ComputerVision::Text::OCR(HttpContent* data, ApiServerRegion region, std::string subscriptionKey, std::string ContentType){

    std::string endpoint = "https://" + ApiServer(region) + "/vision/v2.0/ocr?language=unk&detectOrientation=true";

    std::map<string, string> headers;

    headers.insert(std::pair<std::string, std::string>("Ocp-Apim-Subscription-Key", subscriptionKey));
    headers.insert(std::pair<std::string, std::string>("Content-Type", ContentType));

    HttpResponse response = post(endpoint,"", &headers, data);

    Microsoft::CognitiveServices::ComputerVision::Text::OcrResult analysis;

    CJsonSerializer::Deserialize( &analysis, response.content );

    return analysis;
}
