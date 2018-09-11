#include "description.h"

Description::Description( void ){}
Description::~Description( void ){}

const vector<std::string>& Description::tags() const {
    return _tags;
}

const vector<Caption>& Description::captions() const{
    return _captions;
}


