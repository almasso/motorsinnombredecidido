#include "Resource.h"

Resource::Resource(std::string const& path) :
    size_(-1),
    path_(path){
}

Resource::~Resource() = default;