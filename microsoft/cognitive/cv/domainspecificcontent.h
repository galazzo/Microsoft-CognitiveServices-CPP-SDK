#ifndef DOMAINSPECIFICCONTENT_H
#define DOMAINSPECIFICCONTENT_H

#include "microsoft/cognitive/cv/cv.h"
#include "microsoft/cognitive/cv/description.h"
#include "microsoft/utils/json_serializable.h"
#include <vector>

using namespace std;
using namespace Microsoft::CognitiveServices;
using namespace Microsoft::CognitiveServices::ComputerVision;

class Microsoft::CognitiveServices::ComputerVision::DomainSpecificContent : public IJsonSerializable
{
public:
    DomainSpecificContent();
    virtual ~DomainSpecificContent( void );

    virtual void Serialize( Json::Value& root );
    virtual void Deserialize( Json::Value& root);

    const std::string requestId();
    const Metadata metadata();

    std::vector<Celebrity> celebrities();
    std::vector<Landmark> landmarks();

    void debug();

private:
    std::string _requestId;
    Metadata _metadata;

    std::vector<Celebrity> _celebrities;
    std::vector<Landmark> _landmarks;
};

#endif // DOMAINSPECIFICCONTENT_H
