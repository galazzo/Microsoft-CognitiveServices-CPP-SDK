
#ifndef COGNITIVE_CV_Recognize_Text
#define COGNITIVE_CV_Recognize_Text

#include "microsoft/cognitive/cv/cv.h"
#include "microsoft/utils/json_serializable.h"
#include <vector>

using namespace std;
using namespace Microsoft::CognitiveServices::ComputerVision;

class Microsoft::CognitiveServices::ComputerVision::Text::RecognitionResult : public IJsonSerializable, Microsoft::CognitiveServices::Result {
public:

    RecognitionResult( void );
    virtual ~RecognitionResult( void );

    virtual void Serialize( Json::Value& root );
    virtual void Deserialize( Json::Value& root);

    const std::string status();	    
    std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Line> lines();

    void debug();

private:
    std::string _status;
    std::vector<Microsoft::CognitiveServices::ComputerVision::Text::Line> _lines;    
};


#endif
