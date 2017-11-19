
#ifndef COGNITIVE_CV_DESCRIPTION
#define COGNITIVE_CV_DESCRIPTION

#include "microsoft/cognitive/cv/cv.h"
//#include "microsoft/utils/json_serializable.h"
#include <vector>

using namespace std;
using namespace Microsoft::CognitiveServices::ComputerVision;

class Microsoft::CognitiveServices::ComputerVision::Description
{
public:
    friend class Analysis;

    Description( void );
    virtual ~Description( void );
    const vector<std::string>& tags() const;
    const vector<Caption>&  captions() const;

private:
   vector<std::string> _tags;
   vector<Caption> _captions;
};

//Analysis Microsoft::CognitiveServices::ComputerVision::describe(HttpContent* data, std::string subscriptionKey, std::string ContentType="application/octet-stream");

#endif
