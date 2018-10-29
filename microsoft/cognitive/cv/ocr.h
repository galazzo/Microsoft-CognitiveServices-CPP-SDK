#ifndef OCR_H
#define OCR_H

#include "microsoft/cognitive/cv/cv.h"
#include "microsoft/utils/json_serializable.h"
#include <vector>

using namespace std;
using namespace Microsoft::CognitiveServices::ComputerVision;

struct Microsoft::CognitiveServices::ComputerVision::Text::OcrSettings {
    Microsoft::CognitiveServices::Language Language;
    bool DetectOrientation;
};

class Microsoft::CognitiveServices::ComputerVision::Text::OcrResult : public IJsonSerializable, Microsoft::CognitiveServices::Result {
public:

    OcrResult( void );
    virtual ~OcrResult( void );

    virtual void Serialize( Json::Value& root );
    virtual void Deserialize( Json::Value& root);

    const std::string language();
    double textAngle();
    const std::string orientation();
    std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Region> regions();

    void debug();

private:
    std::string _language;
    double _textAngle;
    std::string _orientation;

    std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Region> _regions;
};

#endif // OCR_H
