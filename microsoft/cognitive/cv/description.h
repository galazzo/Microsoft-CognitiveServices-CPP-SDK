
#ifndef COGNITIVE_CV_DESCRIPTION
#define COGNITIVE_CV_DESCRIPTION

#include "microsoft/cognitive/cv/cv.h"
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

#endif
