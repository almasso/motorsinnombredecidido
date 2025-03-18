#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource {
protected:
    int size_;
    std::string path_;
public:
    explicit Resource(std::string const& path);
    virtual ~Resource();
    virtual bool load() = 0;
    virtual void unload() = 0;
    inline int getSize() const { return size_; }
};


#endif //RESOURCE_H
