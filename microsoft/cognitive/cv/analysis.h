
#ifndef COGNITIVE_CV_ANALYSIS
#define COGNITIVE_CV_ANALYSIS

#include "microsoft/cognitive/cv/cv.h"
#include "microsoft/cognitive/cv/description.h"
#include "microsoft/utils/json_serializable.h"
#include <vector>

using namespace std;
using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision;

class Microsoft::CognitiveServices::ComputerVision::Analysis : public IJsonSerializable {
public:

    Analysis( void );
    virtual ~Analysis( void );

    virtual void Serialize( Json::Value& root );
    virtual void Deserialize( Json::Value& root);

    const std::string requestId();
    const Metadata metadata();
    const Adult adult();
    const Color color();
    const ImageType imageType();
    const vector<Caption>&  captions() const;

    Microsoft::CognitiveServices::ComputerVision::Description description();
    std::vector<Microsoft::CognitiveServices::ComputerVision::Category> categories();
    std::vector<Microsoft::CognitiveServices::ComputerVision::Tag> tags();
    std::vector<Microsoft::CognitiveServices::ComputerVision::Face> faces();

    void debug();

private:
    std::string _requestId;
    Metadata _metadata;
    Adult _adult;
    Color _color;
    ImageType _imageType;
    vector<Caption> _captions;

    Microsoft::CognitiveServices::ComputerVision::Description _description;
    std::vector<Microsoft::CognitiveServices::ComputerVision::Category> _categories;
    std::vector<Microsoft::CognitiveServices::ComputerVision::Tag> _tags;
    std::vector<Microsoft::CognitiveServices::ComputerVision::Face> _faces;
};

#endif
