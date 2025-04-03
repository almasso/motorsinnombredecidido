#ifndef ERROR_H
#define ERROR_H
#include <string>

class Error {
public:
    static void ShowError(const std::string& title, const std::string& body);
};


#endif //ERROR_H
