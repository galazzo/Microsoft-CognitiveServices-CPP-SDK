
#ifndef COGNITIVE_CV_PERSON
#define COGNITIVE_CV_PERSON

#include "microsoft/cognitive/cv/cv.h"

void Microsoft::CognitiveServices::ComputerVision::Person::CreateGroup(HttpContent* data, std::string personGroupId, std::string subscriptionKey, std::string ContentType="application/octet-stream");
void Microsoft::CognitiveServices::ComputerVision::Person::TrainGroup(std::string personGroupId, std::string subscriptionKey, std::string ContentType="application/octet-stream");
void Microsoft::CognitiveServices::ComputerVision::Person::CreatePerson(HttpContent* data, std::string personGroupId, std::string subscriptionKey, std::string ContentType="application/octet-stream");
void Microsoft::CognitiveServices::ComputerVision::Person::AddPersonFace(HttpContent* data, std::string personGroupId, std::string personId,std::string subscriptionKey, std::string ContentType="application/octet-stream");
std::string Microsoft::CognitiveServices::ComputerVision::Person::Identify(HttpContent* data, std::string subscriptionKey, std::string ContentType="application/octet-stream");
std::string Microsoft::CognitiveServices::ComputerVision::Person::Verify(HttpContent* data, std::string subscriptionKey, std::string ContentType="application/octet-stream");

#endif