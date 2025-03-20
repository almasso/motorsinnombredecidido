#include "Resource.h"

Resource::Resource(std::string const& path) :
    _size(-1),
    _path(path){
}

Resource::~Resource() = default;