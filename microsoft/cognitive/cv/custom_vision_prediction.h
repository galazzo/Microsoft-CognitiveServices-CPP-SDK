#ifndef COGNITIVE_CV_CUSTOMVISIONPREDICTION
#define COGNITIVE_CV_CUSTOMVISIONPREDICTION

#include "microsoft/cognitive/cv/cv.h"
#include "microsoft/utils/json_serializable.h"
#include <vector>

using namespace std;
using namespace Microsoft::CognitiveServices;

class Microsoft::CognitiveServices::ComputerVision::CustomVision::Prediction : public IJsonSerializable, Microsoft::CognitiveServices::Result {
public:

    Prediction( void );
    virtual ~Prediction( void );

    virtual void Serialize( Json::Value& root );
    virtual void Deserialize( Json::Value& root);

    const std::string id();
	const std::string project();
	const std::string iteration();
	const std::string created();    
    const std::vector<PredictionBox>&  predictions() const;

    void debug();

private:
    std::string _id;
	std::string _project;
	std::string _iteration;
	std::string _created;
    std::vector<PredictionBox> _predictionBoxes;
};

#endif