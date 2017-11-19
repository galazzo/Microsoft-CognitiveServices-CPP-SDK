#include "description.h"
//#include <cstring>
//#include "microsoft/utils/http.h"
//#include "microsoft/cognitive/cv/analysis.h"
//using namespace std;

Description::Description( void ){}
Description::~Description( void ){}

const vector<std::string>& Description::tags() const {
    return _tags;
}

const vector<Caption>& Description::captions() const{
    return _captions;
}


