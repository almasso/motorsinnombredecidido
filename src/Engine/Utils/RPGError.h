#ifndef RPGERROR_H
#define RPGERROR_H
#include <string>

class RPGError {
public:
    static void ShowError(const std::string& title, const std::string& body);
};


#endif //RPGERROR_H
